#ifdef TWISTED_BACKEND_VULKAN

#include "Twisted/Rendering/Shader.h"
#include "Twisted/Rendering/Vulkan/Shader_Vulkan.h"
#include "Twisted/Rendering/Vulkan/VulkanContext.h"
#include "Twisted/Rendering/Data/ShaderData.h"
#include "Twisted/Rendering/Texture.h"
#include "Utils/GlmUtils.h"
#include "Debug/Logger.h"

#include <shaderc/shaderc.hpp>
#include <stdexcept>
#include <vector>
#include <array>

namespace Twisted
{
    // -----------------------------------------------------------------------
    // GLSL → SPIR-V
    // -----------------------------------------------------------------------
    static std::vector<uint32_t> CompileGlsl(const std::string& source,
                                              shaderc_shader_kind kind,
                                              const char* label)
    {
        shaderc::Compiler        compiler;
        shaderc::CompileOptions  opts;
        opts.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
#ifdef TWISTED_DEBUG
        opts.SetOptimizationLevel(shaderc_optimization_level_zero);
        opts.SetGenerateDebugInfo();
#else
        opts.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif

        auto result = compiler.CompileGlslToSpv(source, kind, label, opts);
        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            TWISTED_ERROR("[Vulkan] Shader '{}' compile error: {}", label, result.GetErrorMessage());
            return {};
        }
        return { result.cbegin(), result.cend() };
    }

    static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<uint32_t>& spv)
    {
        if (spv.empty()) return VK_NULL_HANDLE;

        VkShaderModuleCreateInfo ci{};
        ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        ci.codeSize = spv.size() * sizeof(uint32_t);
        ci.pCode    = spv.data();

        VkShaderModule mod = VK_NULL_HANDLE;
        if (vkCreateShaderModule(device, &ci, nullptr, &mod) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create shader module");
        return mod;
    }

    // -----------------------------------------------------------------------
    // Vertex input — standard interleaved layout pos(3)+normal(3)+uv(2)+color(4)
    // All engine meshes follow this layout (MeshData::Pack()).
    // -----------------------------------------------------------------------
    static constexpr uint32_t k_VertexStride = 12 * sizeof(float); // 3+3+2+4

    static VkVertexInputBindingDescription StandardBinding()
    {
        return { 0, k_VertexStride, VK_VERTEX_INPUT_RATE_VERTEX };
    }

    static std::array<VkVertexInputAttributeDescription, 4> StandardAttributes()
    {
        return {{
            { 0, 0, VK_FORMAT_R32G32B32_SFLOAT,    0 * sizeof(float) },  // position
            { 1, 0, VK_FORMAT_R32G32B32_SFLOAT,    3 * sizeof(float) },  // normal
            { 2, 0, VK_FORMAT_R32G32_SFLOAT,       6 * sizeof(float) },  // texcoord
            { 3, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 8 * sizeof(float) },  // color
        }};
    }

    // -----------------------------------------------------------------------
    // ShaderBackend ctor
    // -----------------------------------------------------------------------
    ShaderBackend::ShaderBackend(const ShaderData& data)
        : m_reflection(data.reflection)
    {
        // Count texture bindings from reflection
        TextureCount = static_cast<uint32_t>(data.reflection.texIndices.size());

        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device == VK_NULL_HANDLE)
        {
            TWISTED_WARN("[Vulkan] ShaderBackend created before device — pipeline deferred");
            return;
        }

        if (!data.vert.has_value() || !data.frag.has_value())
        {
            TWISTED_WARN("[Vulkan] Shader missing vert or frag source — skipping pipeline");
            return;
        }

        // --- Compile GLSL → SPIR-V ---
        auto vertSpv = CompileGlsl(*data.vert, shaderc_vertex_shader,   "vert");
        auto fragSpv = CompileGlsl(*data.frag, shaderc_fragment_shader, "frag");

        if (vertSpv.empty() || fragSpv.empty()) return;

        VkShaderModule vertMod = CreateShaderModule(ctx.Device, vertSpv);
        VkShaderModule fragMod = CreateShaderModule(ctx.Device, fragSpv);

        // --- Descriptor set layout for textures (set 1) ---
        {
            std::vector<VkDescriptorSetLayoutBinding> texBindings(TextureCount);
            for (uint32_t i = 0; i < TextureCount; ++i)
            {
                texBindings[i].binding         = i;
                texBindings[i].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                texBindings[i].descriptorCount = 1;
                texBindings[i].stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT;
            }
            // Always create the layout (even with 0 bindings for non-textured shaders)
            VkDescriptorSetLayoutCreateInfo ci{};
            ci.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            ci.bindingCount = TextureCount;
            ci.pBindings    = texBindings.empty() ? nullptr : texBindings.data();

            if (vkCreateDescriptorSetLayout(ctx.Device, &ci, nullptr, &TextureDescLayout) != VK_SUCCESS)
                throw std::runtime_error("[Vulkan] Failed to create texture descriptor set layout");
        }

        // --- Pipeline layout (set 0 = global MVP+Lights, set 1 = textures) ---
        {
            std::array<VkDescriptorSetLayout, 2> setLayouts = {
                ctx.GlobalDescLayout, TextureDescLayout
            };
            VkPipelineLayoutCreateInfo ci{};
            ci.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            ci.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
            ci.pSetLayouts    = setLayouts.data();

            if (vkCreatePipelineLayout(ctx.Device, &ci, nullptr, &PipelineLayout) != VK_SUCCESS)
                throw std::runtime_error("[Vulkan] Failed to create pipeline layout");
        }

        // --- Per-frame texture descriptor sets ---
        if (TextureCount > 0)
        {
            VkDescriptorPoolSize poolSize{};
            poolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSize.descriptorCount = TextureCount * VK::FramesInFlight;

            VkDescriptorPoolCreateInfo poolCI{};
            poolCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolCI.poolSizeCount = 1;
            poolCI.pPoolSizes    = &poolSize;
            poolCI.maxSets       = VK::FramesInFlight;

            if (vkCreateDescriptorPool(ctx.Device, &poolCI, nullptr, &TextureDescPool) != VK_SUCCESS)
                throw std::runtime_error("[Vulkan] Failed to create texture descriptor pool");

            std::vector<VkDescriptorSetLayout> layouts(VK::FramesInFlight, TextureDescLayout);
            TextureDescSets.resize(VK::FramesInFlight);

            VkDescriptorSetAllocateInfo ai{};
            ai.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            ai.descriptorPool     = TextureDescPool;
            ai.descriptorSetCount = VK::FramesInFlight;
            ai.pSetLayouts        = layouts.data();

            if (vkAllocateDescriptorSets(ctx.Device, &ai, TextureDescSets.data()) != VK_SUCCESS)
                throw std::runtime_error("[Vulkan] Failed to allocate texture descriptor sets");
        }

        // --- Graphics pipeline ---
        VkPipelineShaderStageCreateInfo stages[2]{};
        stages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        stages[0].module = vertMod;
        stages[0].pName  = "main";

        stages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        stages[1].module = fragMod;
        stages[1].pName  = "main";

        auto binding    = StandardBinding();
        auto attributes = StandardAttributes();

        VkPipelineVertexInputStateCreateInfo vertexInput{};
        vertexInput.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInput.vertexBindingDescriptionCount   = 1;
        vertexInput.pVertexBindingDescriptions      = &binding;
        vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
        vertexInput.pVertexAttributeDescriptions    = attributes.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount  = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType            = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.polygonMode      = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth        = 1.0f;
        rasterizer.cullMode         = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace        = VK_FRONT_FACE_COUNTER_CLOCKWISE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable  = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp   = VK_COMPARE_OP_LESS;

        VkPipelineColorBlendAttachmentState blendAttachment{};
        blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                         VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blendAttachment.blendEnable    = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlend{};
        colorBlend.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlend.attachmentCount   = 1;
        colorBlend.pAttachments      = &blendAttachment;

        std::array<VkDynamicState, 2> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates    = dynamicStates.data();

        VkGraphicsPipelineCreateInfo pipelineCI{};
        pipelineCI.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCI.stageCount          = 2;
        pipelineCI.pStages             = stages;
        pipelineCI.pVertexInputState   = &vertexInput;
        pipelineCI.pInputAssemblyState = &inputAssembly;
        pipelineCI.pViewportState      = &viewportState;
        pipelineCI.pRasterizationState = &rasterizer;
        pipelineCI.pMultisampleState   = &multisampling;
        pipelineCI.pDepthStencilState  = &depthStencil;
        pipelineCI.pColorBlendState    = &colorBlend;
        pipelineCI.pDynamicState       = &dynamicState;
        pipelineCI.layout              = PipelineLayout;
        pipelineCI.renderPass          = ctx.RenderPass;
        pipelineCI.subpass             = 0;

        if (vkCreateGraphicsPipelines(ctx.Device, VK_NULL_HANDLE, 1, &pipelineCI,
                                      nullptr, &Pipeline) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create graphics pipeline");

        vkDestroyShaderModule(ctx.Device, vertMod, nullptr);
        vkDestroyShaderModule(ctx.Device, fragMod, nullptr);

        TWISTED_INFO("[Vulkan] Pipeline created ({}tex)", TextureCount);
    }

    ShaderBackend::~ShaderBackend()
    {
        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device == VK_NULL_HANDLE) return;

        if (TextureDescPool   != VK_NULL_HANDLE) vkDestroyDescriptorPool(ctx.Device, TextureDescPool, nullptr);
        if (TextureDescLayout != VK_NULL_HANDLE) vkDestroyDescriptorSetLayout(ctx.Device, TextureDescLayout, nullptr);
        if (Pipeline          != VK_NULL_HANDLE) vkDestroyPipeline(ctx.Device, Pipeline, nullptr);
        if (PipelineLayout    != VK_NULL_HANDLE) vkDestroyPipelineLayout(ctx.Device, PipelineLayout, nullptr);
    }

    bool ShaderBackend::UsesBlock(uint32_t bindingPoint) const
    {
        for (const auto& block : m_reflection.blocks)
        {
            // The ShaderLayouts.h binding points: MVP=0, Lights=1
            (void)block;
        }
        // All Vulkan shaders use the global descriptor set (MVP+Lights always bound)
        return true;
    }

    // -----------------------------------------------------------------------
    // Shader class
    // -----------------------------------------------------------------------
    Shader::Shader(const std::string& name)
        : TObject(name), m_backend(nullptr)
    {
    }

    Shader::~Shader()
    {
    }

    void Shader::Clear()
    {
        m_backend = nullptr;
    }

    void Shader::OnDestroy()
    {
        Clear();
    }

    void Shader::SetData(const ShaderData& data)
    {
        m_backend        = std::make_unique<ShaderBackend>(data);
        m_fileReflection = data.reflection;
    }

    const ShaderReflection& Shader::GetReflection() const
    {
        return m_backend->m_reflection;
    }

    void Shader::SetBool([[maybe_unused]] bool val,           [[maybe_unused]] size_t reflectionIndex) {}
    void Shader::SetInt([[maybe_unused]] int val,             [[maybe_unused]] size_t reflectionIndex) {}
    void Shader::SetFloat([[maybe_unused]] float val,         [[maybe_unused]] size_t reflectionIndex) {}
    void Shader::SetVec3([[maybe_unused]] const Vec3f& val,   [[maybe_unused]] size_t reflectionIndex) {}
    void Shader::SetVec4([[maybe_unused]] const Vec4f& val,   [[maybe_unused]] size_t reflectionIndex) {}
    void Shader::SetMat4([[maybe_unused]] const Mat4x4f& val, [[maybe_unused]] size_t reflectionIndex) {}
    void Shader::SetTexture([[maybe_unused]] const Texture* tex, [[maybe_unused]] size_t reflectionIndex) {}
}

#endif

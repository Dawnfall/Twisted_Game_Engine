#include "Vulkan/LineRenderer.h"
#include "DebugDraw.h"
#include "VulkanContext.h"

#include <shaderc/shaderc.hpp>
#include <array>
#include <cstring>
#include <algorithm>
#include <stdexcept>

namespace Twisted::VK
{
    static constexpr const char* k_VertSrc = R"(
#version 450
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(std140, binding = 0) uniform ShaderMVPBuffer {
    mat4 uView;
    mat4 uProj;
    vec4 uCameraPos;
};
layout(location = 0) out vec4 fragColor;
void main() {
    gl_Position = uProj * uView * vec4(inPos, 1.0);
    fragColor = inColor;
}
)";

    static constexpr const char* k_FragSrc = R"(
#version 450
layout(location = 0) in vec4 fragColor;
layout(location = 0) out vec4 outColor;
void main() { outColor = fragColor; }
)";

    static std::vector<uint32_t> CompileGlsl(const char* src, shaderc_shader_kind kind, const char* label)
    {
        shaderc::Compiler       compiler;
        shaderc::CompileOptions opts;
        opts.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        opts.SetOptimizationLevel(shaderc_optimization_level_performance);

        auto result = compiler.CompileGlslToSpv(src, strlen(src), kind, label, opts);
        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
            throw std::runtime_error(std::string("[LineRenderer] Shader compile error: ") + result.GetErrorMessage());

        return { result.cbegin(), result.cend() };
    }

    static VkShaderModule MakeModule(VkDevice device, const std::vector<uint32_t>& spv)
    {
        VkShaderModuleCreateInfo ci{};
        ci.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        ci.codeSize = spv.size() * sizeof(uint32_t);
        ci.pCode    = spv.data();

        VkShaderModule mod = VK_NULL_HANDLE;
        vkCreateShaderModule(device, &ci, nullptr, &mod);
        return mod;
    }

    void LineRenderer::Init()
    {
        auto& ctx = VulkanContext::Get();
        VkDevice device = ctx.Device.Handle;

        // Per-frame host-coherent vertex buffers (persistently mapped)
        const VkDeviceSize bufSize = k_MaxVertices * sizeof(LineVertex);
        for (int i = 0; i < Render::FramesInFlight; i++)
        {
            ctx.CreateBuffer(bufSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_vertexBuffers[i], m_vertexMemory[i]);
            vkMapMemory(device, m_vertexMemory[i], 0, bufSize, 0, &m_vertexMapped[i]);
        }

        // Pipeline layout — set 0 (GlobalLayout = MVP UBO) only, no push constants
        {
            VkPipelineLayoutCreateInfo ci{};
            ci.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            ci.setLayoutCount = 1;
            ci.pSetLayouts    = &ctx.Descriptors.GlobalLayout;
            if (vkCreatePipelineLayout(device, &ci, nullptr, &m_pipelineLayout) != VK_SUCCESS)
                throw std::runtime_error("[LineRenderer] Failed to create pipeline layout");
        }

        // Compile shaders
        auto vertSpv = CompileGlsl(k_VertSrc, shaderc_vertex_shader,   "line.vert");
        auto fragSpv = CompileGlsl(k_FragSrc, shaderc_fragment_shader, "line.frag");
        VkShaderModule vertMod = MakeModule(device, vertSpv);
        VkShaderModule fragMod = MakeModule(device, fragSpv);

        VkPipelineShaderStageCreateInfo stages[2]{};
        stages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        stages[0].module = vertMod;
        stages[0].pName  = "main";
        stages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        stages[1].module = fragMod;
        stages[1].pName  = "main";

        // Vertex format: Vec3f pos (loc 0, offset 0) + Vec4f color (loc 1, offset 12)
        VkVertexInputBindingDescription binding{};
        binding.binding   = 0;
        binding.stride    = sizeof(LineVertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkVertexInputAttributeDescription attrs[2]{};
        attrs[0] = { 0, 0, VK_FORMAT_R32G32B32_SFLOAT,    0  };
        attrs[1] = { 1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 12 };

        VkPipelineVertexInputStateCreateInfo vertexInput{};
        vertexInput.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInput.vertexBindingDescriptionCount   = 1;
        vertexInput.pVertexBindingDescriptions      = &binding;
        vertexInput.vertexAttributeDescriptionCount = 2;
        vertexInput.pVertexAttributeDescriptions    = attrs;

        VkPipelineInputAssemblyStateCreateInfo ia{};
        ia.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        ia.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

        VkPipelineViewportStateCreateInfo vp{};
        vp.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        vp.viewportCount = 1;
        vp.scissorCount  = 1;

        VkPipelineRasterizationStateCreateInfo rast{};
        rast.sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rast.polygonMode = VK_POLYGON_MODE_FILL;
        rast.lineWidth   = 1.0f;
        rast.cullMode    = VK_CULL_MODE_NONE;
        rast.frontFace   = VK_FRONT_FACE_CLOCKWISE;

        VkPipelineMultisampleStateCreateInfo ms{};
        ms.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // Depth test ON so lines are occluded by geometry, write OFF so lines don't block each other
        VkPipelineDepthStencilStateCreateInfo ds{};
        ds.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        ds.depthTestEnable  = VK_TRUE;
        ds.depthWriteEnable = VK_FALSE;
        ds.depthCompareOp   = VK_COMPARE_OP_LESS_OR_EQUAL;

        VkPipelineColorBlendAttachmentState blendAtt{};
        blendAtt.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blendAtt.blendEnable    = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo blend{};
        blend.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        blend.attachmentCount = 1;
        blend.pAttachments    = &blendAtt;

        std::array<VkDynamicState, 2> dynStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        VkPipelineDynamicStateCreateInfo dyn{};
        dyn.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dyn.dynamicStateCount = static_cast<uint32_t>(dynStates.size());
        dyn.pDynamicStates    = dynStates.data();

        VkGraphicsPipelineCreateInfo pci{};
        pci.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pci.stageCount          = 2;
        pci.pStages             = stages;
        pci.pVertexInputState   = &vertexInput;
        pci.pInputAssemblyState = &ia;
        pci.pViewportState      = &vp;
        pci.pRasterizationState = &rast;
        pci.pMultisampleState   = &ms;
        pci.pDepthStencilState  = &ds;
        pci.pColorBlendState    = &blend;
        pci.pDynamicState       = &dyn;
        pci.layout              = m_pipelineLayout;
        pci.renderPass          = ctx.Swapchain.OffscreenRenderPass;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pci, nullptr, &m_pipeline) != VK_SUCCESS)
            throw std::runtime_error("[LineRenderer] Failed to create pipeline");

        vkDestroyShaderModule(device, vertMod, nullptr);
        vkDestroyShaderModule(device, fragMod, nullptr);
    }

    void LineRenderer::Shutdown()
    {
        auto& ctx = VulkanContext::Get();
        VkDevice device = ctx.Device.Handle;
        if (device == VK_NULL_HANDLE) return;

        vkDeviceWaitIdle(device);

        for (int i = 0; i < Render::FramesInFlight; i++)
        {
            if (m_vertexBuffers[i] == VK_NULL_HANDLE) continue;
            vkUnmapMemory(device, m_vertexMemory[i]);
            vkDestroyBuffer(device, m_vertexBuffers[i], nullptr);
            vkFreeMemory(device, m_vertexMemory[i], nullptr);
            m_vertexBuffers[i] = VK_NULL_HANDLE;
            m_vertexMemory[i]  = VK_NULL_HANDLE;
            m_vertexMapped[i]  = nullptr;
        }

        if (m_pipeline       != VK_NULL_HANDLE) vkDestroyPipeline(device, m_pipeline, nullptr);
        if (m_pipelineLayout != VK_NULL_HANDLE) vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
        m_pipeline       = VK_NULL_HANDLE;
        m_pipelineLayout = VK_NULL_HANDLE;
    }

    void LineRenderer::Render(VkCommandBuffer cmd, VkDescriptorSet globalDescSet)
    {
        if (m_pipeline == VK_NULL_HANDLE) return;

        const auto& lines = DebugDraw::GetLines();
        if (lines.empty()) return;

        const uint32_t frame     = VulkanContext::Get().CurrentFrame;
        const uint32_t vertCount = static_cast<uint32_t>(std::min(lines.size(), (size_t)k_MaxVertices));

        memcpy(m_vertexMapped[frame], lines.data(), vertCount * sizeof(LineVertex));

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_pipelineLayout, 0, 1, &globalDescSet, 0, nullptr);

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(cmd, 0, 1, &m_vertexBuffers[frame], &offset);
        vkCmdDraw(cmd, vertCount, 1, 0, 0);
    }
}

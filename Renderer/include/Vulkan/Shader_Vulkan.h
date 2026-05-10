#ifdef TWISTED_BACKEND_VULKAN
#pragma once

#include <vulkan/vulkan.h>
#include "Data/ShaderData.h"
#include <vector>

namespace Twisted
{
    struct ShaderBackend
    {
        VkPipeline            Pipeline            = VK_NULL_HANDLE;
        VkPipelineLayout      PipelineLayout      = VK_NULL_HANDLE;
        VkDescriptorSetLayout TextureDescLayout   = VK_NULL_HANDLE;
        VkDescriptorPool      TextureDescPool     = VK_NULL_HANDLE;
        std::vector<VkDescriptorSet> TextureDescSets;

        ShaderReflection m_reflection;
        uint32_t         TextureCount = 0;

        ShaderBackend(const ShaderData& data);
        ~ShaderBackend();

        bool IsValid()             const { return Pipeline != VK_NULL_HANDLE; }
        bool UsesBlock(uint32_t bindingPoint) const;
    };
}

#endif

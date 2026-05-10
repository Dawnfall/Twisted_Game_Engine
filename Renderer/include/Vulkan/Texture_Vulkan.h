#ifdef TWISTED_BACKEND_VULKAN
#pragma once

#include <vulkan/vulkan.h>

namespace Twisted
{
    struct TextureBackend
    {
        VkImage        Image      = VK_NULL_HANDLE;
        VkImageView    ImageView  = VK_NULL_HANDLE;
        VkDeviceMemory Memory     = VK_NULL_HANDLE;
        VkSampler      Sampler    = VK_NULL_HANDLE;

        bool IsValid() const { return Image != VK_NULL_HANDLE; }
    };
}

#endif

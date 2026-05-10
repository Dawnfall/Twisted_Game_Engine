#ifdef TWISTED_BACKEND_VULKAN
#pragma once

#include <vulkan/vulkan.h>

namespace Twisted
{
    struct FramebufferBackend
    {
        VkFramebuffer Framebuffer = VK_NULL_HANDLE;
        VkRenderPass  RenderPass  = VK_NULL_HANDLE;

        bool IsValid() const { return Framebuffer != VK_NULL_HANDLE; }
    };
}

#endif

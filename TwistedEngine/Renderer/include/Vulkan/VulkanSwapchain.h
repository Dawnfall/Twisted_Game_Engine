#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace Twisted::VK
{
    struct VulkanDevice;

    struct VulkanSwapchain
    {
        VkSwapchainKHR             Handle           = VK_NULL_HANDLE;
        VkFormat                   Format           = VK_FORMAT_UNDEFINED;
        VkExtent2D                 Extent           = {};
        std::vector<VkImage>       Images;
        std::vector<VkImageView>   ImageViews;
        std::vector<VkFramebuffer> Framebuffers;

        VkImage        DepthImage  = VK_NULL_HANDLE;
        VkDeviceMemory DepthMemory = VK_NULL_HANDLE;
        VkImageView    DepthView   = VK_NULL_HANDLE;
        VkFormat       DepthFormat = VK_FORMAT_UNDEFINED;

        VkRenderPass RenderPass          = VK_NULL_HANDLE;
        VkRenderPass OffscreenRenderPass = VK_NULL_HANDLE;

        void Create(VulkanDevice& dev, uint32_t width, uint32_t height);
        void DestroyResizeable(VkDevice device);
        void Shutdown(VkDevice device);

    private:
        void CreateSwapchain(VulkanDevice& dev, uint32_t width, uint32_t height);
        void CreateImageViews(VkDevice device);
        void CreateDepthResources(VulkanDevice& dev);
        void CreateRenderPass(VkDevice device);
        void CreateOffscreenRenderPass(VkDevice device);
        void CreateFramebuffers(VkDevice device);
    };
}

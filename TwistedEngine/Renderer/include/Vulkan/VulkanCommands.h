#pragma once

#include "RenderConstants.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace Twisted::VK
{
    struct VulkanCommands
    {
        VkCommandPool                Pool    = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> Buffers;

        VkSemaphore ImageAvailableSemaphores[Render::FramesInFlight] = {};
        VkFence     InFlightFences[Render::FramesInFlight]           = {};
        std::vector<VkSemaphore> RenderFinishedSemaphores;

        void Init(VkDevice device, uint32_t graphicsQueueFamily, size_t swapchainImageCount);
        void AllocateCommandBuffers(VkDevice device);
        void CreateRenderFinishedSemaphores(VkDevice device, size_t imageCount);
        void DestroyRenderFinishedSemaphores(VkDevice device);
        void Shutdown(VkDevice device);

        VkCommandBuffer BeginSingleTime(VkDevice device);
        void            EndSingleTime(VkDevice device, VkCommandBuffer cmd, VkQueue graphicsQueue);

        void TransitionImageLayout(VkDevice device, VkQueue graphicsQueue,
                                   VkImage image, VkFormat format,
                                   VkImageLayout oldLayout, VkImageLayout newLayout);

        void CopyBufferToImage(VkDevice device, VkQueue graphicsQueue,
                               VkBuffer buffer, VkImage image,
                               uint32_t width, uint32_t height);

    private:
        void CreatePool(VkDevice device, uint32_t graphicsQueueFamily);
        void CreateSyncObjects(VkDevice device, size_t swapchainImageCount);
    };
}

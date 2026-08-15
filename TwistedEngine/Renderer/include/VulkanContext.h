#pragma once

#include "AppCore.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanSwapchain.h"
#include "Vulkan/VulkanCommands.h"
#include "Vulkan/VulkanDescriptors.h"

struct GLFWwindow;

namespace Twisted::VK
{
    struct TWISTED_API VulkanContext
    {
        VulkanDevice      Device;
        VulkanSwapchain   Swapchain;
        VulkanCommands    Commands;
        VulkanDescriptors Descriptors;

        GLFWwindow* GlfwWindow   = nullptr;
        uint32_t    CurrentFrame = 0;

        VulkanContext(const VulkanContext&)            = delete;
        VulkanContext& operator=(const VulkanContext&) = delete;
        VulkanContext(VulkanContext&&)                 = delete;
        VulkanContext& operator=(VulkanContext&&)      = delete;

        bool IsReady() const { return Swapchain.Handle != VK_NULL_HANDLE; }

        // Convenience wrappers — keep call sites in Mesh/Texture/FrameBuffer unchanged
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties,
                          VkBuffer& buffer, VkDeviceMemory& memory)
        { Device.CreateBuffer(size, usage, properties, buffer, memory); }

        void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties,
                         VkImage& image, VkDeviceMemory& memory)
        { Device.CreateImage(width, height, format, tiling, usage, properties, image, memory); }

        VkCommandBuffer BeginSingleTimeCommands()
        { return Commands.BeginSingleTime(Device.Handle); }

        void EndSingleTimeCommands(VkCommandBuffer cmd)
        { Commands.EndSingleTime(Device.Handle, cmd, Device.GraphicsQueue); }

        void TransitionImageLayout(VkImage image, VkFormat format,
                                   VkImageLayout oldLayout, VkImageLayout newLayout)
        { Commands.TransitionImageLayout(Device.Handle, Device.GraphicsQueue, image, format, oldLayout, newLayout); }

        void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
        { Commands.CopyBufferToImage(Device.Handle, Device.GraphicsQueue, buffer, image, width, height); }

        static VulkanContext& Init(GLFWwindow* window);
        static VulkanContext& Get();

        void RebuildSwapchain(uint32_t width, uint32_t height);
        void Shutdown();

    private:
        VulkanContext()  = default;
        ~VulkanContext() = default;
    };
}

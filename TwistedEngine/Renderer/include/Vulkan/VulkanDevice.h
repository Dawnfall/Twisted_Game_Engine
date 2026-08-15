#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

struct GLFWwindow;

namespace Twisted::VK
{
    struct QueueFamilies
    {
        uint32_t Graphics = UINT32_MAX;
        uint32_t Present  = UINT32_MAX;
        bool IsComplete() const { return Graphics != UINT32_MAX && Present != UINT32_MAX; }
    };

    struct SwapchainSupport
    {
        VkSurfaceCapabilitiesKHR        Capabilities{};
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR>   PresentModes;
    };

    struct VulkanDevice
    {
        VkInstance               Instance       = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT DebugMessenger = VK_NULL_HANDLE;
        VkPhysicalDevice         Physical       = VK_NULL_HANDLE;
        VkDevice                 Handle         = VK_NULL_HANDLE;
        QueueFamilies            Queues;
        VkQueue                  GraphicsQueue  = VK_NULL_HANDLE;
        VkQueue                  PresentQueue   = VK_NULL_HANDLE;
        VkSurfaceKHR             Surface        = VK_NULL_HANDLE;

        void Init(GLFWwindow* window);
        void Shutdown();

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props) const;

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties,
                          VkBuffer& buffer, VkDeviceMemory& memory);

        void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties,
                         VkImage& image, VkDeviceMemory& memory);

        SwapchainSupport QuerySwapchainSupport(VkPhysicalDevice device) const;

    private:
        void CreateInstance();
        void CreateDebugMessenger();
        void CreateSurface(GLFWwindow* window);
        QueueFamilies FindQueueFamilies(VkPhysicalDevice device) const;
        void PickPhysicalDevice();
        void CreateLogicalDevice();
    };
}

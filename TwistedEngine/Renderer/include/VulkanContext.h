#pragma once

#include "AppCore.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

struct GLFWwindow;

namespace Twisted::VK
{
    static constexpr int FramesInFlight = 2;

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

    static constexpr int MaxCamerasPerFrame = 4;

    struct TWISTED_API VulkanContext
    {
        explicit VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        VulkanContext(const VulkanContext&)            = delete;
        VulkanContext& operator=(const VulkanContext&) = delete;
        VulkanContext(VulkanContext&&)                 = delete;
        VulkanContext& operator=(VulkanContext&&)      = delete;

        // Window (needed to query framebuffer size, e.g. on minimize)
        GLFWwindow* GlfwWindow = nullptr;

        // Core
        VkInstance               Instance       = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT DebugMessenger = VK_NULL_HANDLE;
        VkPhysicalDevice         PhysicalDevice = VK_NULL_HANDLE;
        VkDevice                 Device         = VK_NULL_HANDLE;

        // Queues
        QueueFamilies Queues;
        VkQueue GraphicsQueue = VK_NULL_HANDLE;
        VkQueue PresentQueue  = VK_NULL_HANDLE;

        // Surface + swapchain
        VkSurfaceKHR   Surface         = VK_NULL_HANDLE;
        VkSwapchainKHR Swapchain       = VK_NULL_HANDLE;
        VkFormat       SwapchainFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D     SwapchainExtent = {};

        std::vector<VkImage>     SwapchainImages;
        std::vector<VkImageView> SwapchainImageViews;
        std::vector<VkFramebuffer> SwapchainFramebuffers;

        // Depth
        VkImage        DepthImage  = VK_NULL_HANDLE;
        VkDeviceMemory DepthMemory = VK_NULL_HANDLE;
        VkImageView    DepthView   = VK_NULL_HANDLE;
        VkFormat       DepthFormat = VK_FORMAT_UNDEFINED;

        // Render pass + commands
        VkRenderPass  RenderPass         = VK_NULL_HANDLE;  // Swapchain render pass
        VkRenderPass  OffscreenRenderPass = VK_NULL_HANDLE; // R8G8B8A8_UNORM+D24S8, shared by all camera FBs and shader pipelines
        VkCommandPool CommandPool = VK_NULL_HANDLE;

        std::vector<VkCommandBuffer> CommandBuffers;

        // Per-frame sync
        VkSemaphore ImageAvailableSemaphores[FramesInFlight] = {};
        VkFence     InFlightFences[FramesInFlight]           = {};
        uint32_t    CurrentFrame = 0;

        // Per-swapchain-image render-finished semaphores (one per image, not per frame)
        std::vector<VkSemaphore> RenderFinishedSemaphores;

        // Global per-frame UBOs (MVP binding=0, Lights binding=1)
        VkDescriptorSetLayout        GlobalDescLayout = VK_NULL_HANDLE;
        VkDescriptorPool             GlobalDescPool   = VK_NULL_HANDLE;
        std::vector<VkDescriptorSet> GlobalDescSets;

        std::vector<VkBuffer>       MvpUBOs,       LightsUBOs;
        std::vector<VkDeviceMemory> MvpUBOMemory,  LightsUBOMemory;
        std::vector<void*>          MvpUBOMapped,  LightsUBOMapped;
        VkDeviceSize                MvpSlotStride = 0; // aligned size of one per-camera MVP slot

        bool IsReady() const { return Swapchain != VK_NULL_HANDLE; }

        static VulkanContext& Init(GLFWwindow* window);
        static VulkanContext& Get();

        void RebuildSwapchain(uint32_t width, uint32_t height);
        void Shutdown();

        // -------------------------------------------------------
        // Utilities — used by Mesh/Texture/Shader backends
        // -------------------------------------------------------
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties,
                          VkBuffer& buffer, VkDeviceMemory& memory);

        void CreateImage(uint32_t width, uint32_t height, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties,
                         VkImage& image, VkDeviceMemory& memory);

        VkCommandBuffer BeginSingleTimeCommands();
        void            EndSingleTimeCommands(VkCommandBuffer cmd);

        void TransitionImageLayout(VkImage image, VkFormat format,
                                   VkImageLayout oldLayout, VkImageLayout newLayout);

        void CopyBufferToImage(VkBuffer buffer, VkImage image,
                               uint32_t width, uint32_t height);

    private:
        void CreateInstance();
        void CreateDebugMessenger();
        void CreateSurface(GLFWwindow* window);
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapchain(uint32_t width, uint32_t height);
        void CreateImageViews();
        void CreateDepthResources();
        void CreateRenderPass();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();
        void CreateSwapchainFramebuffers();
        void CreateGlobalDescriptorLayout();
        void CreateGlobalUniformBuffers();
        void CreateGlobalDescriptorPool();
        void CreateGlobalDescriptorSets();
        void CreateOffscreenRenderPass();
        void DestroySwapchainResources();

        QueueFamilies    FindQueueFamilies(VkPhysicalDevice device) const;
        SwapchainSupport QuerySwapchainSupport(VkPhysicalDevice device) const;
        uint32_t         FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props) const;
    };

} // namespace Twisted::VK

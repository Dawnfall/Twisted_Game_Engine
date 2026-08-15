#include "VulkanContext.h"
#include "Debug/Logger.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Twisted::VK
{

static VulkanContext* s_instance = nullptr;

VulkanContext& VulkanContext::Init(GLFWwindow* window)
{
    static VulkanContext s_ctx;
    s_instance = &s_ctx;

    s_ctx.GlfwWindow = window;
    s_ctx.Device.Init(window);

    int w = 1280, h = 720;
    if (window)
        glfwGetFramebufferSize(window, &w, &h);

    s_ctx.Swapchain.Create(s_ctx.Device,
                           static_cast<uint32_t>(w),
                           static_cast<uint32_t>(h));
    s_ctx.Commands.Init(s_ctx.Device.Handle,
                        s_ctx.Device.Queues.Graphics,
                        s_ctx.Swapchain.Images.size());
    s_ctx.Descriptors.Init(s_ctx.Device);

    TWISTED_INFO("[Vulkan] Bootstrap complete — {}x{}", w, h);
    return s_ctx;
}

VulkanContext& VulkanContext::Get()
{
    TWISTED_ASSERT(s_instance != nullptr, "[Vulkan] VulkanContext not initialized — call Init() first");
    return *s_instance;
}

void VulkanContext::RebuildSwapchain(uint32_t width, uint32_t height)
{
    vkDeviceWaitIdle(Device.Handle);

    Commands.DestroyRenderFinishedSemaphores(Device.Handle);
    Swapchain.DestroyResizeable(Device.Handle);
    Swapchain.Create(Device, width, height);
    Commands.AllocateCommandBuffers(Device.Handle);
    Commands.CreateRenderFinishedSemaphores(Device.Handle, Swapchain.Images.size());
    Descriptors.RebuildSets(Device.Handle);
}

void VulkanContext::Shutdown()
{
    if (Device.Handle == VK_NULL_HANDLE) return;
    vkDeviceWaitIdle(Device.Handle);

    Commands.Shutdown(Device.Handle);
    Descriptors.Shutdown(Device.Handle);
    Swapchain.Shutdown(Device.Handle);
    Device.Shutdown();

    s_instance = nullptr;
    TWISTED_INFO("[Vulkan] Shutdown complete");
}

} // namespace Twisted::VK

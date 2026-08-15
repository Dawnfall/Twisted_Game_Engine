#include "Vulkan/VulkanCommands.h"
#include "Debug/Logger.h"

#include <stdexcept>

namespace Twisted::VK
{

void VulkanCommands::Init(VkDevice device, uint32_t graphicsQueueFamily, size_t swapchainImageCount)
{
    CreatePool(device, graphicsQueueFamily);
    AllocateCommandBuffers(device);
    CreateSyncObjects(device, swapchainImageCount);
}

void VulkanCommands::AllocateCommandBuffers(VkDevice device)
{
    Buffers.resize(Render::FramesInFlight);

    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool        = Pool;
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = static_cast<uint32_t>(Buffers.size());

    if (vkAllocateCommandBuffers(device, &ai, Buffers.data()) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate command buffers");
}

void VulkanCommands::CreateRenderFinishedSemaphores(VkDevice device, size_t imageCount)
{
    VkSemaphoreCreateInfo semCI{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    RenderFinishedSemaphores.resize(imageCount);
    for (auto& sem : RenderFinishedSemaphores)
        if (vkCreateSemaphore(device, &semCI, nullptr, &sem) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create render-finished semaphore");
}

void VulkanCommands::DestroyRenderFinishedSemaphores(VkDevice device)
{
    for (auto& sem : RenderFinishedSemaphores)
        if (sem != VK_NULL_HANDLE)
            vkDestroySemaphore(device, sem, nullptr);
    RenderFinishedSemaphores.clear();
}

void VulkanCommands::Shutdown(VkDevice device)
{
    for (int i = 0; i < Render::FramesInFlight; ++i)
    {
        if (ImageAvailableSemaphores[i]) vkDestroySemaphore(device, ImageAvailableSemaphores[i], nullptr);
        if (InFlightFences[i])           vkDestroyFence(device, InFlightFences[i], nullptr);
    }
    DestroyRenderFinishedSemaphores(device);
    if (Pool != VK_NULL_HANDLE)
        vkDestroyCommandPool(device, Pool, nullptr);
}

void VulkanCommands::CreatePool(VkDevice device, uint32_t graphicsQueueFamily)
{
    VkCommandPoolCreateInfo ci{};
    ci.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    ci.queueFamilyIndex = graphicsQueueFamily;
    ci.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &ci, nullptr, &Pool) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create command pool");
}

void VulkanCommands::CreateSyncObjects(VkDevice device, size_t swapchainImageCount)
{
    VkSemaphoreCreateInfo semCI{};
    semCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCI{};
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < Render::FramesInFlight; ++i)
    {
        if (vkCreateSemaphore(device, &semCI,   nullptr, &ImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence    (device, &fenceCI, nullptr, &InFlightFences[i])           != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create sync objects");
    }

    CreateRenderFinishedSemaphores(device, swapchainImageCount);
}

VkCommandBuffer VulkanCommands::BeginSingleTime(VkDevice device)
{
    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandPool        = Pool;
    ai.commandBufferCount = 1;

    VkCommandBuffer cmd = VK_NULL_HANDLE;
    vkAllocateCommandBuffers(device, &ai, &cmd);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &beginInfo);

    return cmd;
}

void VulkanCommands::EndSingleTime(VkDevice device, VkCommandBuffer cmd, VkQueue graphicsQueue)
{
    vkEndCommandBuffer(cmd);

    VkSubmitInfo submit{};
    submit.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers    = &cmd;

    vkQueueSubmit(graphicsQueue, 1, &submit, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
    vkFreeCommandBuffers(device, Pool, 1, &cmd);
}

void VulkanCommands::TransitionImageLayout(VkDevice device, VkQueue graphicsQueue,
                                           VkImage image, VkFormat format,
                                           VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer cmd = BeginSingleTime(device);

    VkImageMemoryBarrier barrier{};
    barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout           = oldLayout;
    barrier.newLayout           = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image               = image;

    barrier.subresourceRange.aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL &&
                                           (format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
                                            format == VK_FORMAT_D24_UNORM_S8_UINT))
                                        ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
                                        : VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;

    VkPipelineStageFlags srcStage = 0, dstStage = 0;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
             newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else
    {
        barrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    }

    vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    EndSingleTime(device, cmd, graphicsQueue);
}

void VulkanCommands::CopyBufferToImage(VkDevice device, VkQueue graphicsQueue,
                                       VkBuffer buffer, VkImage image,
                                       uint32_t width, uint32_t height)
{
    VkCommandBuffer cmd = BeginSingleTime(device);

    VkBufferImageCopy region{};
    region.bufferOffset                    = 0;
    region.bufferRowLength                 = 0;
    region.bufferImageHeight               = 0;
    region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel       = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount     = 1;
    region.imageOffset                     = { 0, 0, 0 };
    region.imageExtent                     = { width, height, 1 };

    vkCmdCopyBufferToImage(cmd, buffer, image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    EndSingleTime(device, cmd, graphicsQueue);
}

} // namespace Twisted::VK

#include "Vulkan/VulkanSwapchain.h"
#include "Vulkan/VulkanDevice.h"
#include "Debug/Logger.h"

#include <stdexcept>
#include <algorithm>
#include <limits>
#include <array>

namespace Twisted::VK
{

void VulkanSwapchain::Create(VulkanDevice& dev, uint32_t width, uint32_t height)
{
    CreateSwapchain(dev, width, height);
    CreateImageViews(dev.Handle);
    CreateDepthResources(dev);
    if (RenderPass == VK_NULL_HANDLE)
        CreateRenderPass(dev.Handle);
    if (OffscreenRenderPass == VK_NULL_HANDLE)
        CreateOffscreenRenderPass(dev.Handle);
    CreateFramebuffers(dev.Handle);
}

void VulkanSwapchain::DestroyResizeable(VkDevice device)
{
    for (auto& fb : Framebuffers)
        vkDestroyFramebuffer(device, fb, nullptr);
    Framebuffers.clear();

    if (DepthView   != VK_NULL_HANDLE) { vkDestroyImageView(device, DepthView,   nullptr); DepthView   = VK_NULL_HANDLE; }
    if (DepthImage  != VK_NULL_HANDLE) { vkDestroyImage(device, DepthImage,       nullptr); DepthImage  = VK_NULL_HANDLE; }
    if (DepthMemory != VK_NULL_HANDLE) { vkFreeMemory(device, DepthMemory,         nullptr); DepthMemory = VK_NULL_HANDLE; }

    for (auto& iv : ImageViews)
        vkDestroyImageView(device, iv, nullptr);
    ImageViews.clear();
}

void VulkanSwapchain::Shutdown(VkDevice device)
{
    DestroyResizeable(device);

    if (OffscreenRenderPass != VK_NULL_HANDLE) { vkDestroyRenderPass(device, OffscreenRenderPass, nullptr); OffscreenRenderPass = VK_NULL_HANDLE; }
    if (RenderPass          != VK_NULL_HANDLE) { vkDestroyRenderPass(device, RenderPass,          nullptr); RenderPass          = VK_NULL_HANDLE; }
    if (Handle              != VK_NULL_HANDLE) { vkDestroySwapchainKHR(device, Handle,            nullptr); Handle              = VK_NULL_HANDLE; }
}

void VulkanSwapchain::CreateSwapchain(VulkanDevice& dev, uint32_t width, uint32_t height)
{
    SwapchainSupport support = dev.QuerySwapchainSupport(dev.Physical);

    VkSurfaceFormatKHR fmt = support.Formats[0];
    for (const auto& f : support.Formats)
        if (f.format == VK_FORMAT_B8G8R8A8_UNORM &&
            f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) { fmt = f; break; }

    VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto& m : support.PresentModes)
        if (m == VK_PRESENT_MODE_MAILBOX_KHR) { mode = m; break; }

    VkExtent2D extent;
    if (support.Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        extent = support.Capabilities.currentExtent;
    }
    else
    {
        extent.width  = std::clamp(width,  support.Capabilities.minImageExtent.width,
                                           support.Capabilities.maxImageExtent.width);
        extent.height = std::clamp(height, support.Capabilities.minImageExtent.height,
                                           support.Capabilities.maxImageExtent.height);
    }

    uint32_t imageCount = support.Capabilities.minImageCount + 1;
    if (support.Capabilities.maxImageCount > 0)
        imageCount = std::min(imageCount, support.Capabilities.maxImageCount);

    VkSwapchainCreateInfoKHR ci{};
    ci.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    ci.surface          = dev.Surface;
    ci.minImageCount    = imageCount;
    ci.imageFormat      = fmt.format;
    ci.imageColorSpace  = fmt.colorSpace;
    ci.imageExtent      = extent;
    ci.imageArrayLayers = 1;
    ci.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilies[] = { dev.Queues.Graphics, dev.Queues.Present };
    if (dev.Queues.Graphics != dev.Queues.Present)
    {
        ci.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        ci.queueFamilyIndexCount = 2;
        ci.pQueueFamilyIndices   = queueFamilies;
    }
    else
    {
        ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    ci.preTransform   = support.Capabilities.currentTransform;
    ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    ci.presentMode    = mode;
    ci.clipped        = VK_TRUE;
    ci.oldSwapchain   = Handle;

    VkSwapchainKHR newSwapchain = VK_NULL_HANDLE;
    if (vkCreateSwapchainKHR(dev.Handle, &ci, nullptr, &newSwapchain) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create swapchain");

    if (Handle != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(dev.Handle, Handle, nullptr);
    Handle = newSwapchain;

    uint32_t count = 0;
    vkGetSwapchainImagesKHR(dev.Handle, Handle, &count, nullptr);
    Images.resize(count);
    vkGetSwapchainImagesKHR(dev.Handle, Handle, &count, Images.data());

    Format = fmt.format;
    Extent = extent;
}

void VulkanSwapchain::CreateImageViews(VkDevice device)
{
    ImageViews.resize(Images.size());
    for (size_t i = 0; i < Images.size(); ++i)
    {
        VkImageViewCreateInfo ci{};
        ci.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ci.image        = Images[i];
        ci.viewType     = VK_IMAGE_VIEW_TYPE_2D;
        ci.format       = Format;
        ci.components   = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                             VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
        ci.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        ci.subresourceRange.baseMipLevel   = 0;
        ci.subresourceRange.levelCount     = 1;
        ci.subresourceRange.baseArrayLayer = 0;
        ci.subresourceRange.layerCount     = 1;

        if (vkCreateImageView(device, &ci, nullptr, &ImageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create swapchain image view");
    }
}

void VulkanSwapchain::CreateDepthResources(VulkanDevice& dev)
{
    const std::array candidates = {
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT
    };
    DepthFormat = VK_FORMAT_UNDEFINED;
    for (VkFormat candidate : candidates)
    {
        VkFormatProperties props{};
        vkGetPhysicalDeviceFormatProperties(dev.Physical, candidate, &props);
        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            DepthFormat = candidate;
            break;
        }
    }
    if (DepthFormat == VK_FORMAT_UNDEFINED)
        throw std::runtime_error("[Vulkan] No supported depth format");

    dev.CreateImage(Extent.width, Extent.height, DepthFormat,
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    DepthImage, DepthMemory);

    VkImageViewCreateInfo viewCI{};
    viewCI.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCI.image                           = DepthImage;
    viewCI.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    viewCI.format                          = DepthFormat;
    viewCI.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewCI.subresourceRange.baseMipLevel   = 0;
    viewCI.subresourceRange.levelCount     = 1;
    viewCI.subresourceRange.baseArrayLayer = 0;
    viewCI.subresourceRange.layerCount     = 1;

    if (vkCreateImageView(dev.Handle, &viewCI, nullptr, &DepthView) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create depth image view");
}

void VulkanSwapchain::CreateRenderPass(VkDevice device)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format         = Format;
    colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format         = DepthFormat;
    depthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorRef{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference depthRef{ 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorRef;
    subpass.pDepthStencilAttachment = &depthRef;

    VkSubpassDependency dep{};
    dep.srcSubpass    = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass    = 0;
    dep.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.srcAccessMask = 0;
    dep.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

    VkRenderPassCreateInfo ci{};
    ci.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    ci.attachmentCount = static_cast<uint32_t>(attachments.size());
    ci.pAttachments    = attachments.data();
    ci.subpassCount    = 1;
    ci.pSubpasses      = &subpass;
    ci.dependencyCount = 1;
    ci.pDependencies   = &dep;

    if (vkCreateRenderPass(device, &ci, nullptr, &RenderPass) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create render pass");
}

void VulkanSwapchain::CreateOffscreenRenderPass(VkDevice device)
{
    VkAttachmentDescription colorAtt{};
    colorAtt.format         = VK_FORMAT_R8G8B8A8_UNORM;
    colorAtt.samples        = VK_SAMPLE_COUNT_1_BIT;
    colorAtt.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAtt.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    colorAtt.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAtt.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAtt.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAtt.finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentDescription depthAtt{};
    depthAtt.format         = VK_FORMAT_D24_UNORM_S8_UINT;
    depthAtt.samples        = VK_SAMPLE_COUNT_1_BIT;
    depthAtt.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAtt.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAtt.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAtt.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAtt.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAtt.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorRef{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference depthRef{ 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorRef;
    subpass.pDepthStencilAttachment = &depthRef;

    std::array<VkSubpassDependency, 2> deps{};
    deps[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
    deps[0].dstSubpass      = 0;
    deps[0].srcStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    deps[0].srcAccessMask   = VK_ACCESS_SHADER_READ_BIT;
    deps[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                               VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    deps[0].dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    deps[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    deps[1].srcSubpass      = 0;
    deps[1].dstSubpass      = VK_SUBPASS_EXTERNAL;
    deps[1].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    deps[1].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    deps[1].dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    deps[1].dstAccessMask   = VK_ACCESS_SHADER_READ_BIT;
    deps[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAtt, depthAtt };

    VkRenderPassCreateInfo ci{};
    ci.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    ci.attachmentCount = static_cast<uint32_t>(attachments.size());
    ci.pAttachments    = attachments.data();
    ci.subpassCount    = 1;
    ci.pSubpasses      = &subpass;
    ci.dependencyCount = static_cast<uint32_t>(deps.size());
    ci.pDependencies   = deps.data();

    if (vkCreateRenderPass(device, &ci, nullptr, &OffscreenRenderPass) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create offscreen render pass");
}

void VulkanSwapchain::CreateFramebuffers(VkDevice device)
{
    Framebuffers.resize(ImageViews.size());
    for (size_t i = 0; i < ImageViews.size(); ++i)
    {
        std::array<VkImageView, 2> attachments = { ImageViews[i], DepthView };

        VkFramebufferCreateInfo ci{};
        ci.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        ci.renderPass      = RenderPass;
        ci.attachmentCount = static_cast<uint32_t>(attachments.size());
        ci.pAttachments    = attachments.data();
        ci.width           = Extent.width;
        ci.height          = Extent.height;
        ci.layers          = 1;

        if (vkCreateFramebuffer(device, &ci, nullptr, &Framebuffers[i]) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create swapchain framebuffer");
    }
}

} // namespace Twisted::VK

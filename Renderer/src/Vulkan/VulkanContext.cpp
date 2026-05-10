#ifdef TWISTED_BACKEND_VULKAN

#include "Twisted/Rendering/Vulkan/VulkanContext.h"
#include "Debug/Logger.h"

#include <stdexcept>
#include <algorithm>
#include <limits>
#include <array>
#include <set>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif

namespace Twisted::VK
{

// -----------------------------------------------------------------------
// Validation
// -----------------------------------------------------------------------
static const std::vector<const char*> k_ValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static const std::vector<const char*> k_DeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef TWISTED_DEBUG
static constexpr bool k_EnableValidation = true;
#else
static constexpr bool k_EnableValidation = false;
#endif

// -----------------------------------------------------------------------
// Debug messenger
// -----------------------------------------------------------------------
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void*)
{
    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        TWISTED_ERROR("[Vulkan] {}", data->pMessage);
    else if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        TWISTED_WARN("[Vulkan] {}", data->pMessage);
    else
        TWISTED_TRACE("[Vulkan] {}", data->pMessage);
    return VK_FALSE;
}

static VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCI,
    const VkAllocationCallbacks* pAlloc,
    VkDebugUtilsMessengerEXT* pOut)
{
    auto fn = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    return fn ? fn(instance, pCI, pAlloc, pOut) : VK_ERROR_EXTENSION_NOT_PRESENT;
}

static void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* pAlloc)
{
    auto fn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (fn) fn(instance, messenger, pAlloc);
}

// -----------------------------------------------------------------------
// Singleton
// -----------------------------------------------------------------------
VulkanContext& VulkanContext::Get()
{
    static VulkanContext s_ctx;
    return s_ctx;
}

// -----------------------------------------------------------------------
// Public bootstrap
// -----------------------------------------------------------------------
void VulkanContext::InitSurface(void* hwnd, void* hinstance)
{
    CreateInstance();
    if (k_EnableValidation) CreateDebugMessenger();
    CreateSurface(hwnd, hinstance);
    PickPhysicalDevice();
    CreateLogicalDevice();

    uint32_t width = 1280, height = 720;
#ifdef _WIN32
    if (hwnd)
    {
        RECT rect{};
        GetClientRect(static_cast<HWND>(hwnd), &rect);
        width  = static_cast<uint32_t>(rect.right  - rect.left);
        height = static_cast<uint32_t>(rect.bottom - rect.top);
    }
#endif
    CreateSwapchain(width, height);
    CreateImageViews();
    CreateDepthResources();
    CreateRenderPass();
    CreateSwapchainFramebuffers();
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSyncObjects();
    CreateGlobalDescriptorLayout();
    CreateGlobalUniformBuffers();
    CreateGlobalDescriptorPool();
    CreateGlobalDescriptorSets();

    TWISTED_INFO("[Vulkan] Bootstrap complete — {}x{}", width, height);
}

void VulkanContext::RebuildSwapchain(uint32_t width, uint32_t height)
{
    vkDeviceWaitIdle(Device);
    DestroySwapchainResources();
    CreateSwapchain(width, height);
    CreateImageViews();
    CreateDepthResources();
    CreateRenderPass();
    CreateSwapchainFramebuffers();
    CreateCommandBuffers();
    CreateGlobalDescriptorSets();
}

// -----------------------------------------------------------------------
// Instance
// -----------------------------------------------------------------------
void VulkanContext::CreateInstance()
{
    if (k_EnableValidation)
    {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> layers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

        for (const char* name : k_ValidationLayers)
        {
            bool found = false;
            for (const auto& layer : layers)
                if (strcmp(layer.layerName, name) == 0) { found = true; break; }
            if (!found)
                TWISTED_WARN("[Vulkan] Validation layer '{}' not available", name);
        }
    }

    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "TwistedEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "TwistedEngine";
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_3;

    std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#else
        "VK_KHR_xcb_surface",
#endif
    };
    if (k_EnableValidation)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    VkDebugUtilsMessengerCreateInfoEXT debugCI{};
    debugCI.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCI.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCI.pfnUserCallback = DebugCallback;

    VkInstanceCreateInfo ci{};
    ci.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pApplicationInfo        = &appInfo;
    ci.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    ci.ppEnabledExtensionNames = extensions.data();

    if (k_EnableValidation)
    {
        ci.enabledLayerCount   = static_cast<uint32_t>(k_ValidationLayers.size());
        ci.ppEnabledLayerNames = k_ValidationLayers.data();
        ci.pNext               = &debugCI;
    }

    if (vkCreateInstance(&ci, nullptr, &Instance) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create VkInstance");

    TWISTED_INFO("[Vulkan] Instance created");
}

// -----------------------------------------------------------------------
// Debug messenger
// -----------------------------------------------------------------------
void VulkanContext::CreateDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT ci{};
    ci.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    ci.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    ci.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    ci.pfnUserCallback = DebugCallback;

    CreateDebugUtilsMessengerEXT(Instance, &ci, nullptr, &DebugMessenger);
}

// -----------------------------------------------------------------------
// Surface
// -----------------------------------------------------------------------
void VulkanContext::CreateSurface(void* hwnd, void* hinstance)
{
#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR ci{};
    ci.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    ci.hwnd      = static_cast<HWND>(hwnd);
    ci.hinstance = hinstance ? static_cast<HINSTANCE>(hinstance)
                              : GetModuleHandle(nullptr);

    if (vkCreateWin32SurfaceKHR(Instance, &ci, nullptr, &Surface) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create Win32 surface");
    TWISTED_INFO("[Vulkan] Win32 surface created");
#else
    (void)hwnd; (void)hinstance;
    TWISTED_WARN("[Vulkan] Surface creation not implemented for this platform");
#endif
}

// -----------------------------------------------------------------------
// Physical device helpers
// -----------------------------------------------------------------------
QueueFamilies VulkanContext::FindQueueFamilies(VkPhysicalDevice device) const
{
    QueueFamilies result;
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());

    for (uint32_t i = 0; i < count; ++i)
    {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            result.Graphics = i;

        VkBool32 present = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Surface, &present);
        if (present)
            result.Present = i;

        if (result.IsComplete()) break;
    }
    return result;
}

SwapchainSupport VulkanContext::QuerySwapchainSupport(VkPhysicalDevice device) const
{
    SwapchainSupport support;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Surface, &support.Capabilities);

    uint32_t count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &count, nullptr);
    if (count)
    {
        support.Formats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, Surface, &count, support.Formats.data());
    }

    count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &count, nullptr);
    if (count)
    {
        support.PresentModes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, Surface, &count, support.PresentModes.data());
    }

    return support;
}

static bool DeviceSupportsExtensions(VkPhysicalDevice device)
{
    uint32_t count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> available(count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, available.data());

    std::set<std::string> required(k_DeviceExtensions.begin(), k_DeviceExtensions.end());
    for (const auto& ext : available)
        required.erase(ext.extensionName);
    return required.empty();
}

void VulkanContext::PickPhysicalDevice()
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(Instance, &count, nullptr);
    if (!count)
        throw std::runtime_error("[Vulkan] No GPUs with Vulkan support found");

    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(Instance, &count, devices.data());

    for (VkPhysicalDevice dev : devices)
    {
        QueueFamilies  qf    = FindQueueFamilies(dev);
        bool           extOk = DeviceSupportsExtensions(dev);
        bool           scOk  = false;
        if (extOk)
        {
            SwapchainSupport sc = QuerySwapchainSupport(dev);
            scOk = !sc.Formats.empty() && !sc.PresentModes.empty();
        }

        if (qf.IsComplete() && extOk && scOk)
        {
            PhysicalDevice = dev;
            Queues         = qf;
            break;
        }
    }

    if (PhysicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("[Vulkan] Failed to find a suitable GPU");

    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(PhysicalDevice, &props);
    TWISTED_INFO("[Vulkan] GPU selected: {}", props.deviceName);
}

// -----------------------------------------------------------------------
// Logical device
// -----------------------------------------------------------------------
void VulkanContext::CreateLogicalDevice()
{
    std::set<uint32_t> uniqueQueues = { Queues.Graphics, Queues.Present };

    float priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueCIs;
    for (uint32_t qf : uniqueQueues)
    {
        VkDeviceQueueCreateInfo ci{};
        ci.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        ci.queueFamilyIndex = qf;
        ci.queueCount       = 1;
        ci.pQueuePriorities = &priority;
        queueCIs.push_back(ci);
    }

    VkPhysicalDeviceFeatures features{};
    features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo ci{};
    ci.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    ci.queueCreateInfoCount    = static_cast<uint32_t>(queueCIs.size());
    ci.pQueueCreateInfos       = queueCIs.data();
    ci.enabledExtensionCount   = static_cast<uint32_t>(k_DeviceExtensions.size());
    ci.ppEnabledExtensionNames = k_DeviceExtensions.data();
    ci.pEnabledFeatures        = &features;

    if (k_EnableValidation)
    {
        ci.enabledLayerCount   = static_cast<uint32_t>(k_ValidationLayers.size());
        ci.ppEnabledLayerNames = k_ValidationLayers.data();
    }

    if (vkCreateDevice(PhysicalDevice, &ci, nullptr, &Device) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create logical device");

    vkGetDeviceQueue(Device, Queues.Graphics, 0, &GraphicsQueue);
    vkGetDeviceQueue(Device, Queues.Present,  0, &PresentQueue);

    TWISTED_INFO("[Vulkan] Logical device created");
}

// -----------------------------------------------------------------------
// Swapchain
// -----------------------------------------------------------------------
void VulkanContext::CreateSwapchain(uint32_t width, uint32_t height)
{
    SwapchainSupport support = QuerySwapchainSupport(PhysicalDevice);

    // Prefer BGRA8 SRGB
    VkSurfaceFormatKHR fmt = support.Formats[0];
    for (const auto& f : support.Formats)
        if (f.format == VK_FORMAT_B8G8R8A8_SRGB &&
            f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) { fmt = f; break; }

    // Prefer mailbox (triple-buffer); FIFO always available
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
    ci.surface          = Surface;
    ci.minImageCount    = imageCount;
    ci.imageFormat      = fmt.format;
    ci.imageColorSpace  = fmt.colorSpace;
    ci.imageExtent      = extent;
    ci.imageArrayLayers = 1;
    ci.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilies[] = { Queues.Graphics, Queues.Present };
    if (Queues.Graphics != Queues.Present)
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
    ci.oldSwapchain   = Swapchain;

    VkSwapchainKHR newSwapchain = VK_NULL_HANDLE;
    if (vkCreateSwapchainKHR(Device, &ci, nullptr, &newSwapchain) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create swapchain");

    if (Swapchain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(Device, Swapchain, nullptr);
    Swapchain = newSwapchain;

    uint32_t count = 0;
    vkGetSwapchainImagesKHR(Device, Swapchain, &count, nullptr);
    SwapchainImages.resize(count);
    vkGetSwapchainImagesKHR(Device, Swapchain, &count, SwapchainImages.data());

    SwapchainFormat = fmt.format;
    SwapchainExtent = extent;
}

// -----------------------------------------------------------------------
// Image views
// -----------------------------------------------------------------------
void VulkanContext::CreateImageViews()
{
    SwapchainImageViews.resize(SwapchainImages.size());
    for (size_t i = 0; i < SwapchainImages.size(); ++i)
    {
        VkImageViewCreateInfo ci{};
        ci.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ci.image        = SwapchainImages[i];
        ci.viewType     = VK_IMAGE_VIEW_TYPE_2D;
        ci.format       = SwapchainFormat;
        ci.components   = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                             VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
        ci.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        ci.subresourceRange.baseMipLevel   = 0;
        ci.subresourceRange.levelCount     = 1;
        ci.subresourceRange.baseArrayLayer = 0;
        ci.subresourceRange.layerCount     = 1;

        if (vkCreateImageView(Device, &ci, nullptr, &SwapchainImageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create swapchain image view");
    }
}

// -----------------------------------------------------------------------
// Depth
// -----------------------------------------------------------------------
uint32_t VulkanContext::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props) const
{
    VkPhysicalDeviceMemoryProperties memProps{};
    vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memProps);

    for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
        if ((typeFilter & (1u << i)) &&
            (memProps.memoryTypes[i].propertyFlags & props) == props)
            return i;

    throw std::runtime_error("[Vulkan] Failed to find suitable memory type");
}

void VulkanContext::CreateDepthResources()
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
        vkGetPhysicalDeviceFormatProperties(PhysicalDevice, candidate, &props);
        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            DepthFormat = candidate;
            break;
        }
    }
    if (DepthFormat == VK_FORMAT_UNDEFINED)
        throw std::runtime_error("[Vulkan] No supported depth format");

    VkImageCreateInfo imgCI{};
    imgCI.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imgCI.imageType     = VK_IMAGE_TYPE_2D;
    imgCI.format        = DepthFormat;
    imgCI.extent        = { SwapchainExtent.width, SwapchainExtent.height, 1 };
    imgCI.mipLevels     = 1;
    imgCI.arrayLayers   = 1;
    imgCI.samples       = VK_SAMPLE_COUNT_1_BIT;
    imgCI.tiling        = VK_IMAGE_TILING_OPTIMAL;
    imgCI.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imgCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(Device, &imgCI, nullptr, &DepthImage) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create depth image");

    VkMemoryRequirements memReqs{};
    vkGetImageMemoryRequirements(Device, DepthImage, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memReqs.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memReqs.memoryTypeBits,
                                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(Device, &allocInfo, nullptr, &DepthMemory) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate depth memory");
    vkBindImageMemory(Device, DepthImage, DepthMemory, 0);

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

    if (vkCreateImageView(Device, &viewCI, nullptr, &DepthView) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create depth image view");
}

// -----------------------------------------------------------------------
// Render pass
// -----------------------------------------------------------------------
void VulkanContext::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format         = SwapchainFormat;
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

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;
    colorRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthRef{};
    depthRef.attachment = 1;
    depthRef.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

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

    if (vkCreateRenderPass(Device, &ci, nullptr, &RenderPass) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create render pass");
}

// -----------------------------------------------------------------------
// Command pool + buffers
// -----------------------------------------------------------------------
void VulkanContext::CreateCommandPool()
{
    VkCommandPoolCreateInfo ci{};
    ci.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    ci.queueFamilyIndex = Queues.Graphics;
    ci.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(Device, &ci, nullptr, &CommandPool) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create command pool");
}

void VulkanContext::CreateCommandBuffers()
{
    CommandBuffers.resize(FramesInFlight);

    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool        = CommandPool;
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = static_cast<uint32_t>(CommandBuffers.size());

    if (vkAllocateCommandBuffers(Device, &ai, CommandBuffers.data()) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate command buffers");
}

// -----------------------------------------------------------------------
// Sync objects
// -----------------------------------------------------------------------
void VulkanContext::CreateSyncObjects()
{
    VkSemaphoreCreateInfo semCI{};
    semCI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCI{};
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < FramesInFlight; ++i)
    {
        if (vkCreateSemaphore(Device, &semCI,   nullptr, &ImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(Device, &semCI,   nullptr, &RenderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence    (Device, &fenceCI, nullptr, &InFlightFences[i])           != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create sync objects");
    }
}

// -----------------------------------------------------------------------
// Cleanup helpers
// -----------------------------------------------------------------------
void VulkanContext::DestroySwapchainResources()
{
    for (auto& fb : SwapchainFramebuffers)
        vkDestroyFramebuffer(Device, fb, nullptr);
    SwapchainFramebuffers.clear();

    if (DepthView != VK_NULL_HANDLE)
        { vkDestroyImageView(Device, DepthView, nullptr);   DepthView   = VK_NULL_HANDLE; }
    if (DepthImage != VK_NULL_HANDLE)
        { vkDestroyImage(Device, DepthImage, nullptr);      DepthImage  = VK_NULL_HANDLE; }
    if (DepthMemory != VK_NULL_HANDLE)
        { vkFreeMemory(Device, DepthMemory, nullptr);       DepthMemory = VK_NULL_HANDLE; }

    for (auto& iv : SwapchainImageViews)
        vkDestroyImageView(Device, iv, nullptr);
    SwapchainImageViews.clear();

    if (RenderPass != VK_NULL_HANDLE)
        { vkDestroyRenderPass(Device, RenderPass, nullptr); RenderPass  = VK_NULL_HANDLE; }
}

// -----------------------------------------------------------------------
// Swapchain framebuffers (one per swapchain image)
// -----------------------------------------------------------------------
void VulkanContext::CreateSwapchainFramebuffers()
{
    SwapchainFramebuffers.resize(SwapchainImageViews.size());
    for (size_t i = 0; i < SwapchainImageViews.size(); ++i)
    {
        std::array<VkImageView, 2> attachments = { SwapchainImageViews[i], DepthView };

        VkFramebufferCreateInfo ci{};
        ci.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        ci.renderPass      = RenderPass;
        ci.attachmentCount = static_cast<uint32_t>(attachments.size());
        ci.pAttachments    = attachments.data();
        ci.width           = SwapchainExtent.width;
        ci.height          = SwapchainExtent.height;
        ci.layers          = 1;

        if (vkCreateFramebuffer(Device, &ci, nullptr, &SwapchainFramebuffers[i]) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create swapchain framebuffer");
    }
}

// -----------------------------------------------------------------------
// Utility: buffer allocation
// -----------------------------------------------------------------------
void VulkanContext::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                 VkMemoryPropertyFlags properties,
                                 VkBuffer& buffer, VkDeviceMemory& memory)
{
    VkBufferCreateInfo ci{};
    ci.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    ci.size        = size;
    ci.usage       = usage;
    ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Device, &ci, nullptr, &buffer) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create buffer");

    VkMemoryRequirements memReqs{};
    vkGetBufferMemoryRequirements(Device, buffer, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memReqs.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memReqs.memoryTypeBits, properties);

    if (vkAllocateMemory(Device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate buffer memory");

    vkBindBufferMemory(Device, buffer, memory, 0);
}

// -----------------------------------------------------------------------
// Utility: image allocation
// -----------------------------------------------------------------------
void VulkanContext::CreateImage(uint32_t width, uint32_t height, VkFormat format,
                                VkImageTiling tiling, VkImageUsageFlags usage,
                                VkMemoryPropertyFlags properties,
                                VkImage& image, VkDeviceMemory& memory)
{
    VkImageCreateInfo ci{};
    ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ci.imageType     = VK_IMAGE_TYPE_2D;
    ci.extent        = { width, height, 1 };
    ci.mipLevels     = 1;
    ci.arrayLayers   = 1;
    ci.format        = format;
    ci.tiling        = tiling;
    ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ci.usage         = usage;
    ci.samples       = VK_SAMPLE_COUNT_1_BIT;
    ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(Device, &ci, nullptr, &image) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create image");

    VkMemoryRequirements memReqs{};
    vkGetImageMemoryRequirements(Device, image, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memReqs.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memReqs.memoryTypeBits, properties);

    if (vkAllocateMemory(Device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate image memory");

    vkBindImageMemory(Device, image, memory, 0);
}

// -----------------------------------------------------------------------
// Utility: single-use command buffer
// -----------------------------------------------------------------------
VkCommandBuffer VulkanContext::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandPool        = CommandPool;
    ai.commandBufferCount = 1;

    VkCommandBuffer cmd = VK_NULL_HANDLE;
    vkAllocateCommandBuffers(Device, &ai, &cmd);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &beginInfo);

    return cmd;
}

void VulkanContext::EndSingleTimeCommands(VkCommandBuffer cmd)
{
    vkEndCommandBuffer(cmd);

    VkSubmitInfo submit{};
    submit.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers    = &cmd;

    vkQueueSubmit(GraphicsQueue, 1, &submit, VK_NULL_HANDLE);
    vkQueueWaitIdle(GraphicsQueue);
    vkFreeCommandBuffers(Device, CommandPool, 1, &cmd);
}

// -----------------------------------------------------------------------
// Utility: image layout transition
// -----------------------------------------------------------------------
void VulkanContext::TransitionImageLayout(VkImage image, VkFormat format,
                                          VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer cmd = BeginSingleTimeCommands();

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
    EndSingleTimeCommands(cmd);
}

// -----------------------------------------------------------------------
// Utility: copy staging buffer into image
// -----------------------------------------------------------------------
void VulkanContext::CopyBufferToImage(VkBuffer buffer, VkImage image,
                                      uint32_t width, uint32_t height)
{
    VkCommandBuffer cmd = BeginSingleTimeCommands();

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
    EndSingleTimeCommands(cmd);
}

// -----------------------------------------------------------------------
// Global descriptor layout (MVP @ binding 0, Lights @ binding 1)
// -----------------------------------------------------------------------
void VulkanContext::CreateGlobalDescriptorLayout()
{
    std::array<VkDescriptorSetLayoutBinding, 2> bindings{};

    bindings[0].binding            = 0;
    bindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount    = 1;
    bindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;

    bindings[1].binding            = 1;
    bindings[1].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[1].descriptorCount    = 1;
    bindings[1].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo ci{};
    ci.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    ci.bindingCount = static_cast<uint32_t>(bindings.size());
    ci.pBindings    = bindings.data();

    if (vkCreateDescriptorSetLayout(Device, &ci, nullptr, &GlobalDescLayout) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create global descriptor set layout");
}

// -----------------------------------------------------------------------
// Global UBOs (persistently mapped, one per frame in flight)
// -----------------------------------------------------------------------
void VulkanContext::CreateGlobalUniformBuffers()
{
    const VkDeviceSize mvpSize    = 208;   // sizeof(ShaderMVPBuffer)
    const VkDeviceSize lightsSize = 5568;  // sizeof(ShaderLightsBuffer)

    MvpUBOs.resize(FramesInFlight);
    MvpUBOMemory.resize(FramesInFlight);
    MvpUBOMapped.resize(FramesInFlight);

    LightsUBOs.resize(FramesInFlight);
    LightsUBOMemory.resize(FramesInFlight);
    LightsUBOMapped.resize(FramesInFlight);

    for (int i = 0; i < FramesInFlight; ++i)
    {
        CreateBuffer(mvpSize,
                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     MvpUBOs[i], MvpUBOMemory[i]);
        vkMapMemory(Device, MvpUBOMemory[i], 0, mvpSize, 0, &MvpUBOMapped[i]);

        CreateBuffer(lightsSize,
                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     LightsUBOs[i], LightsUBOMemory[i]);
        vkMapMemory(Device, LightsUBOMemory[i], 0, lightsSize, 0, &LightsUBOMapped[i]);
    }
}

void VulkanContext::CreateGlobalDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(FramesInFlight) * 2; // MVP + Lights
    poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(FramesInFlight) * 64; // textures

    VkDescriptorPoolCreateInfo ci{};
    ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    ci.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    ci.pPoolSizes    = poolSizes.data();
    ci.maxSets       = static_cast<uint32_t>(FramesInFlight) * 32;
    ci.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    if (vkCreateDescriptorPool(Device, &ci, nullptr, &GlobalDescPool) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create global descriptor pool");
}

void VulkanContext::CreateGlobalDescriptorSets()
{
    if (GlobalDescLayout == VK_NULL_HANDLE || GlobalDescPool == VK_NULL_HANDLE)
        return;

    // Free old sets if rebuilding
    if (!GlobalDescSets.empty())
        vkFreeDescriptorSets(Device, GlobalDescPool,
                             static_cast<uint32_t>(GlobalDescSets.size()),
                             GlobalDescSets.data());

    std::vector<VkDescriptorSetLayout> layouts(FramesInFlight, GlobalDescLayout);
    GlobalDescSets.resize(FramesInFlight);

    VkDescriptorSetAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    ai.descriptorPool     = GlobalDescPool;
    ai.descriptorSetCount = static_cast<uint32_t>(layouts.size());
    ai.pSetLayouts        = layouts.data();

    if (vkAllocateDescriptorSets(Device, &ai, GlobalDescSets.data()) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate global descriptor sets");

    for (int i = 0; i < FramesInFlight; ++i)
    {
        VkDescriptorBufferInfo mvpInfo{};
        mvpInfo.buffer = MvpUBOs[i];
        mvpInfo.offset = 0;
        mvpInfo.range  = 208;

        VkDescriptorBufferInfo lightsInfo{};
        lightsInfo.buffer = LightsUBOs[i];
        lightsInfo.offset = 0;
        lightsInfo.range  = 5568;

        std::array<VkWriteDescriptorSet, 2> writes{};
        writes[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[0].dstSet          = GlobalDescSets[i];
        writes[0].dstBinding      = 0;
        writes[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[0].descriptorCount = 1;
        writes[0].pBufferInfo     = &mvpInfo;

        writes[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet          = GlobalDescSets[i];
        writes[1].dstBinding      = 1;
        writes[1].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[1].descriptorCount = 1;
        writes[1].pBufferInfo     = &lightsInfo;

        vkUpdateDescriptorSets(Device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
    }
}

void VulkanContext::Shutdown()
{
    if (Device == VK_NULL_HANDLE) return;
    vkDeviceWaitIdle(Device);

    for (int i = 0; i < FramesInFlight; ++i)
    {
        if (ImageAvailableSemaphores[i])
            vkDestroySemaphore(Device, ImageAvailableSemaphores[i], nullptr);
        if (RenderFinishedSemaphores[i])
            vkDestroySemaphore(Device, RenderFinishedSemaphores[i], nullptr);
        if (InFlightFences[i])
            vkDestroyFence(Device, InFlightFences[i], nullptr);
    }

    if (GlobalDescPool != VK_NULL_HANDLE)
        { vkDestroyDescriptorPool(Device, GlobalDescPool, nullptr);  GlobalDescPool = VK_NULL_HANDLE; }
    if (GlobalDescLayout != VK_NULL_HANDLE)
        { vkDestroyDescriptorSetLayout(Device, GlobalDescLayout, nullptr); GlobalDescLayout = VK_NULL_HANDLE; }

    for (int i = 0; i < FramesInFlight; ++i)
    {
        if (MvpUBOs[i])    { vkUnmapMemory(Device, MvpUBOMemory[i]); vkDestroyBuffer(Device, MvpUBOs[i], nullptr);    vkFreeMemory(Device, MvpUBOMemory[i], nullptr); }
        if (LightsUBOs[i]) { vkUnmapMemory(Device, LightsUBOMemory[i]); vkDestroyBuffer(Device, LightsUBOs[i], nullptr); vkFreeMemory(Device, LightsUBOMemory[i], nullptr); }
    }

    if (CommandPool != VK_NULL_HANDLE)
        vkDestroyCommandPool(Device, CommandPool, nullptr);

    DestroySwapchainResources();

    if (Swapchain  != VK_NULL_HANDLE) { vkDestroySwapchainKHR(Device,   Swapchain,  nullptr); Swapchain  = VK_NULL_HANDLE; }
    if (Surface    != VK_NULL_HANDLE) { vkDestroySurfaceKHR(Instance,   Surface,    nullptr); Surface    = VK_NULL_HANDLE; }
    if (Device     != VK_NULL_HANDLE) { vkDestroyDevice(Device, nullptr);                      Device     = VK_NULL_HANDLE; }

    if (k_EnableValidation && DebugMessenger != VK_NULL_HANDLE)
        DestroyDebugUtilsMessengerEXT(Instance, DebugMessenger, nullptr);

    if (Instance   != VK_NULL_HANDLE) { vkDestroyInstance(Instance, nullptr);                  Instance   = VK_NULL_HANDLE; }

    TWISTED_INFO("[Vulkan] Shutdown complete");
}

} // namespace Twisted::VK

#endif // TWISTED_BACKEND_VULKAN

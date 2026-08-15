#include "Vulkan/VulkanDevice.h"
#include "Debug/Logger.h"

#include <stdexcept>
#include <set>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Twisted::VK
{

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
// Debug messenger helpers
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

// -----------------------------------------------------------------------
// VulkanDevice
// -----------------------------------------------------------------------
void VulkanDevice::Init(GLFWwindow* window)
{
    CreateInstance();
    if (k_EnableValidation)
        CreateDebugMessenger();
    CreateSurface(window);
    PickPhysicalDevice();
    CreateLogicalDevice();
}

void VulkanDevice::Shutdown()
{
    if (k_EnableValidation && DebugMessenger != VK_NULL_HANDLE)
        DestroyDebugUtilsMessengerEXT(Instance, DebugMessenger, nullptr);

    if (Surface   != VK_NULL_HANDLE) { vkDestroySurfaceKHR(Instance, Surface, nullptr);  Surface   = VK_NULL_HANDLE; }
    if (Handle    != VK_NULL_HANDLE) { vkDestroyDevice(Handle, nullptr);                 Handle    = VK_NULL_HANDLE; }
    if (Instance  != VK_NULL_HANDLE) { vkDestroyInstance(Instance, nullptr);             Instance  = VK_NULL_HANDLE; }
}

void VulkanDevice::CreateInstance()
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

    uint32_t glfwExtCount = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    std::vector<const char*> extensions(glfwExts, glfwExts + glfwExtCount);
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

void VulkanDevice::CreateDebugMessenger()
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

void VulkanDevice::CreateSurface(GLFWwindow* window)
{
    if (glfwCreateWindowSurface(Instance, window, nullptr, &Surface) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create window surface");
    TWISTED_INFO("[Vulkan] Window surface created");
}

QueueFamilies VulkanDevice::FindQueueFamilies(VkPhysicalDevice device) const
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

SwapchainSupport VulkanDevice::QuerySwapchainSupport(VkPhysicalDevice device) const
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

void VulkanDevice::PickPhysicalDevice()
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
            Physical = dev;
            Queues   = qf;
            break;
        }
    }

    if (Physical == VK_NULL_HANDLE)
        throw std::runtime_error("[Vulkan] Failed to find a suitable GPU");

    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(Physical, &props);
    TWISTED_INFO("[Vulkan] GPU selected: {}", props.deviceName);
}

void VulkanDevice::CreateLogicalDevice()
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

    if (vkCreateDevice(Physical, &ci, nullptr, &Handle) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create logical device");

    vkGetDeviceQueue(Handle, Queues.Graphics, 0, &GraphicsQueue);
    vkGetDeviceQueue(Handle, Queues.Present,  0, &PresentQueue);

    TWISTED_INFO("[Vulkan] Logical device created");
}

uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props) const
{
    VkPhysicalDeviceMemoryProperties memProps{};
    vkGetPhysicalDeviceMemoryProperties(Physical, &memProps);

    for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
        if ((typeFilter & (1u << i)) &&
            (memProps.memoryTypes[i].propertyFlags & props) == props)
            return i;

    throw std::runtime_error("[Vulkan] Failed to find suitable memory type");
}

void VulkanDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                VkMemoryPropertyFlags properties,
                                VkBuffer& buffer, VkDeviceMemory& memory)
{
    VkBufferCreateInfo ci{};
    ci.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    ci.size        = size;
    ci.usage       = usage;
    ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Handle, &ci, nullptr, &buffer) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create buffer");

    VkMemoryRequirements memReqs{};
    vkGetBufferMemoryRequirements(Handle, buffer, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memReqs.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memReqs.memoryTypeBits, properties);

    if (vkAllocateMemory(Handle, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate buffer memory");

    vkBindBufferMemory(Handle, buffer, memory, 0);
}

void VulkanDevice::CreateImage(uint32_t width, uint32_t height, VkFormat format,
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

    if (vkCreateImage(Handle, &ci, nullptr, &image) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to create image");

    VkMemoryRequirements memReqs{};
    vkGetImageMemoryRequirements(Handle, image, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memReqs.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memReqs.memoryTypeBits, properties);

    if (vkAllocateMemory(Handle, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        throw std::runtime_error("[Vulkan] Failed to allocate image memory");

    vkBindImageMemory(Handle, image, memory, 0);
}

} // namespace Twisted::VK

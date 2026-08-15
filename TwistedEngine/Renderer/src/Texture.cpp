#include "Texture.h"
#include "VulkanContext.h"
#include "Data/TextureData.h"
#include "Debug/Logger.h"

#include <cstring>
#include <stdexcept>

namespace Twisted
{
    // -----------------------------------------------------------------------
    // Helpers
    // -----------------------------------------------------------------------
    static VkFormat ToVkFormat(TextureFormat fmt)
    {
        switch (fmt)
        {
        case TextureFormat::RGBA8:           return VK_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::RGBA8_SRGB:      return VK_FORMAT_R8G8B8A8_SRGB;
        case TextureFormat::RGBA16F:         return VK_FORMAT_R16G16B16A16_SFLOAT;
        case TextureFormat::RGBA32F:         return VK_FORMAT_R32G32B32A32_SFLOAT;
        case TextureFormat::Depth24Stencil8: return VK_FORMAT_D24_UNORM_S8_UINT;
        default:                             return VK_FORMAT_R8G8B8A8_UNORM;
        }
    }

    static VkFilter ToVkMagFilter(TextureMagFilter f)
    {
        return (f == TextureMagFilter::NEAREST) ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
    }

    static VkFilter ToVkMinFilter(TextureMinFilter f)
    {
        switch (f)
        {
        case TextureMinFilter::NEAREST:
        case TextureMinFilter::NEAREST_MIPMAP_NEAREST:
        case TextureMinFilter::NEAREST_MIPMAP_LINEAR:
            return VK_FILTER_NEAREST;
        default:
            return VK_FILTER_LINEAR;
        }
    }

    static VkSamplerAddressMode ToVkWrap(TextureWrap w)
    {
        switch (w)
        {
        case TextureWrap::REPEAT:          return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case TextureWrap::MIRROR_REPEAT:   return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case TextureWrap::CLAMP_TO_EDGE:   return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case TextureWrap::CLAMP_TO_BORDER: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        default:                           return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    }

    static void CreateSampler(VkDevice device, const TextureParams& params, VkSampler& sampler)
    {
        VkSamplerCreateInfo ci{};
        ci.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        ci.magFilter               = ToVkMagFilter(params.MagFilter);
        ci.minFilter               = ToVkMinFilter(params.MinFilter);
        ci.addressModeU            = ToVkWrap(params.Wrap);
        ci.addressModeV            = ToVkWrap(params.Wrap);
        ci.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        ci.anisotropyEnable        = VK_FALSE;
        ci.maxAnisotropy           = 1.0f;
        ci.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        ci.unnormalizedCoordinates = VK_FALSE;
        ci.compareEnable           = VK_FALSE;
        ci.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        if (vkCreateSampler(device, &ci, nullptr, &sampler) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create texture sampler");
    }

    static void CreateImageView(VkDevice device, VkImage image, VkFormat format,
                                VkImageAspectFlags aspect, VkImageView& view)
    {
        VkImageViewCreateInfo ci{};
        ci.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ci.image                           = image;
        ci.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        ci.format                          = format;
        ci.components                      = { VK_COMPONENT_SWIZZLE_IDENTITY,
                                               VK_COMPONENT_SWIZZLE_IDENTITY,
                                               VK_COMPONENT_SWIZZLE_IDENTITY,
                                               VK_COMPONENT_SWIZZLE_IDENTITY };
        ci.subresourceRange.aspectMask     = aspect;
        ci.subresourceRange.baseMipLevel   = 0;
        ci.subresourceRange.levelCount     = 1;
        ci.subresourceRange.baseArrayLayer = 0;
        ci.subresourceRange.layerCount     = 1;

        if (vkCreateImageView(device, &ci, nullptr, &view) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create texture image view");
    }

    // -----------------------------------------------------------------------
    // Texture class
    // -----------------------------------------------------------------------
    Texture::Texture(const std::string& name)
        : TObject(name)
    {
    }

    Texture::~Texture() = default;

    void Texture::Clear()
    {
        if (!IsValid()) return;

        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device.Handle == VK_NULL_HANDLE) return;

        vkDeviceWaitIdle(ctx.Device.Handle);

        if (Sampler   != VK_NULL_HANDLE) vkDestroySampler(ctx.Device.Handle, Sampler, nullptr);
        if (ImageView != VK_NULL_HANDLE) vkDestroyImageView(ctx.Device.Handle, ImageView, nullptr);
        if (Image     != VK_NULL_HANDLE) vkDestroyImage(ctx.Device.Handle, Image, nullptr);
        if (Memory    != VK_NULL_HANDLE) vkFreeMemory(ctx.Device.Handle, Memory, nullptr);

        Image     = VK_NULL_HANDLE;
        ImageView = VK_NULL_HANDLE;
        Memory    = VK_NULL_HANDLE;
        Sampler   = VK_NULL_HANDLE;

        Info.Width = Info.Height = 0;
        Info.MipLevels = 0;
        Version++;
    }

    void Texture::OnDestroy()
    {
        Clear();
    }

    void Texture::SetData(const TextureData& data, TextureParams params)
    {
        Clear();

        Params      = params;
        Info.Width  = data.Width;
        Info.Height = data.Height;
        Info.Format = TextureFormat::RGBA8;
        Info.MipLevels = 1;

        auto& ctx = VK::VulkanContext::Get();
        const VkFormat fmt    = VK_FORMAT_R8G8B8A8_UNORM;
        const VkDeviceSize sz = static_cast<VkDeviceSize>(data.Width) * data.Height * 4;

        VkBuffer       stagingBuf;
        VkDeviceMemory stagingMem;
        ctx.CreateBuffer(sz,
                         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuf, stagingMem);

        void* mapped = nullptr;
        vkMapMemory(ctx.Device.Handle, stagingMem, 0, sz, 0, &mapped);
        memcpy(mapped, data.Data, static_cast<size_t>(sz));
        vkUnmapMemory(ctx.Device.Handle, stagingMem);

        ctx.CreateImage(static_cast<uint32_t>(data.Width),
                        static_cast<uint32_t>(data.Height),
                        fmt, VK_IMAGE_TILING_OPTIMAL,
                        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        Image, Memory);

        ctx.TransitionImageLayout(Image, fmt,
                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        ctx.CopyBufferToImage(stagingBuf, Image,
                              static_cast<uint32_t>(data.Width),
                              static_cast<uint32_t>(data.Height));
        ctx.TransitionImageLayout(Image, fmt,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(ctx.Device.Handle, stagingBuf, nullptr);
        vkFreeMemory(ctx.Device.Handle, stagingMem, nullptr);

        CreateImageView(ctx.Device.Handle, Image, fmt, VK_IMAGE_ASPECT_COLOR_BIT, ImageView);
        CreateSampler(ctx.Device.Handle, params, Sampler);

        Version++;
    }

    void Texture::Resize(const Vec2i& newSize)
    {
        if (newSize.x <= 0 || newSize.y <= 0) return;
        if (Info.Width == newSize.x && Info.Height == newSize.y && IsValid()) return;

        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device.Handle == VK_NULL_HANDLE) return;

        vkDeviceWaitIdle(ctx.Device.Handle);

        const bool isDepth = (Info.UsageFlags & TextureUsage_DepthAttachment) != 0;

        const VkFormat fmt = ToVkFormat(Info.Format);
        VkImageUsageFlags usage = 0;
        if (Info.UsageFlags & TextureUsage_ColorAttachment) usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (Info.UsageFlags & TextureUsage_DepthAttachment) usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        if (Info.UsageFlags & TextureUsage_Sampled)         usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
        if (Info.UsageFlags & TextureUsage_TransferSrc)     usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        if (Info.UsageFlags & TextureUsage_TransferDst)     usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        if (ImageView != VK_NULL_HANDLE) vkDestroyImageView(ctx.Device.Handle, ImageView, nullptr);
        if (Image     != VK_NULL_HANDLE) vkDestroyImage(ctx.Device.Handle, Image, nullptr);
        if (Memory    != VK_NULL_HANDLE) vkFreeMemory(ctx.Device.Handle, Memory, nullptr);

        Info.Width  = newSize.x;
        Info.Height = newSize.y;

        ctx.CreateImage(static_cast<uint32_t>(newSize.x), static_cast<uint32_t>(newSize.y),
                        fmt, VK_IMAGE_TILING_OPTIMAL, usage,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        Image, Memory);

        VkImageAspectFlags aspect = isDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        CreateImageView(ctx.Device.Handle, Image, fmt, aspect, ImageView);

        if (Sampler == VK_NULL_HANDLE)
            CreateSampler(ctx.Device.Handle, Params, Sampler);

        if (Info.UsageFlags & TextureUsage_Sampled)
            ctx.TransitionImageLayout(Image, fmt,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        Version++;
    }
}

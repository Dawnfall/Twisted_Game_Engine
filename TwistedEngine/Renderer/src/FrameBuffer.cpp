#include "FrameBuffer.h"
#include "Texture.h"
#include "VulkanContext.h"
#include "Application/TObject.h"
#include "Debug/Logger.h"

#include <array>
#include <stdexcept>

namespace Twisted
{
    static VkFormat FBToVkFormat(TextureFormat fmt)
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

    Framebuffer::Framebuffer(const std::string& name)
        : TObject(name)
    {
    }

    Framebuffer::~Framebuffer()
    {
    }

    void Framebuffer::OnCreate()
    {
        Color.OwnedDesc.Format     = TextureFormat::RGBA8;
        Color.OwnedDesc.UsageFlags = TextureUsage_ColorAttachment | TextureUsage_Sampled;

        Depth.OwnedDesc.Format     = TextureFormat::Depth24Stencil8;
        Depth.OwnedDesc.UsageFlags = TextureUsage_DepthAttachment;

        Color.Owned = WPtr<Texture>(TObject::Create<Texture>(GetName() + "_color"));
        Depth.Owned = WPtr<Texture>(TObject::Create<Texture>(GetName() + "_depth"));

        Color.Owned->Info = Color.OwnedDesc;
        Depth.Owned->Info = Depth.OwnedDesc;

        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device == VK_NULL_HANDLE) return;

        // Borrow the shared offscreen render pass — must NOT be destroyed in OnDestroy.
        // It is compatible with shader pipelines (same R8G8B8A8_UNORM + D24S8 formats).
        VkRP = ctx.OffscreenRenderPass;

        // Per-FB render pass creation is commented out. The shared OffscreenRenderPass is used
        // instead so that shader pipelines (created against it) remain render-pass-compatible.
        //
        // const VkFormat colorFmt = FBToVkFormat(Color.OwnedDesc.Format);
        // const VkFormat depthFmt = FBToVkFormat(Depth.OwnedDesc.Format);
        // ... (VkRenderPassCreateInfo + vkCreateRenderPass) ...
    }

    void Framebuffer::OnDestroy()
    {
        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device != VK_NULL_HANDLE)
            vkDeviceWaitIdle(ctx.Device);

        // Destroy framebuffer before textures — it references their image views.
        if (VkFB != VK_NULL_HANDLE && ctx.Device != VK_NULL_HANDLE)
            vkDestroyFramebuffer(ctx.Device, VkFB, nullptr);
        VkFB = VK_NULL_HANDLE;

        TObject::Destroy(Color.Owned.get());
        TObject::Destroy(Depth.Owned.get());
        Color.Owned = nullptr;
        Depth.Owned = nullptr;

        // VkRP is borrowed from VulkanContext::OffscreenRenderPass — owned and destroyed there.
        // if (VkRP != VK_NULL_HANDLE && ctx.Device != VK_NULL_HANDLE)
        //     vkDestroyRenderPass(ctx.Device, VkRP, nullptr);
        VkRP = VK_NULL_HANDLE;
    }

    void Framebuffer::SetSize(const Vec2i& newSize)
    {
        if (newSize.x <= 0 || newSize.y <= 0)
            return;
        if (newSize == Size && IsValid())
            return;

        Size = newSize;

        if (Texture* colorTex = Color.Owned.get())
            colorTex->Resize(newSize);
        if (Texture* depthTex = Depth.Owned.get())
            depthTex->Resize(newSize);

        auto& ctx = VK::VulkanContext::Get();
        if (ctx.Device == VK_NULL_HANDLE || VkRP == VK_NULL_HANDLE) return;

        Texture* colorTex = Color.Owned.get();
        Texture* depthTex = Depth.Owned.get();
        if (!colorTex || !depthTex || !colorTex->IsValid() || !depthTex->IsValid()) return;

        vkDeviceWaitIdle(ctx.Device);

        if (VkFB != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(ctx.Device, VkFB, nullptr);
            VkFB = VK_NULL_HANDLE;
        }

        std::array<VkImageView, 2> views = { colorTex->ImageView, depthTex->ImageView };

        VkFramebufferCreateInfo fbCI{};
        fbCI.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbCI.renderPass      = VkRP;
        fbCI.attachmentCount = static_cast<uint32_t>(views.size());
        fbCI.pAttachments    = views.data();
        fbCI.width           = static_cast<uint32_t>(newSize.x);
        fbCI.height          = static_cast<uint32_t>(newSize.y);
        fbCI.layers          = 1;

        if (vkCreateFramebuffer(ctx.Device, &fbCI, nullptr, &VkFB) != VK_SUCCESS)
            throw std::runtime_error("[Vulkan] Failed to create framebuffer");

        Version++;
    }

}

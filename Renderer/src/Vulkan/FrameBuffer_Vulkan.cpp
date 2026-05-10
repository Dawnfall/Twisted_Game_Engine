#ifdef TWISTED_BACKEND_VULKAN

#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/Vulkan/FrameBuffer_Vulkan.h"
#include "Twisted/Rendering/Texture.h"
#include "Twisted/TObject.h"
#include "Debug/Logger.h"

namespace Twisted
{
    Framebuffer::Framebuffer(const std::string& name)
        : TObject(name), m_backend(std::make_unique<FramebufferBackend>())
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

        m_backend = std::make_unique<FramebufferBackend>();
        TWISTED_WARN("Vulkan Framebuffer::OnCreate(): render pass / framebuffer creation not yet implemented");
    }

    void Framebuffer::OnDestroy()
    {
        TObject::Destroy(Color.Owned.get());
        TObject::Destroy(Depth.Owned.get());
        Color.Owned = nullptr;
        Depth.Owned = nullptr;
        // TODO: vkDestroyFramebuffer, vkDestroyRenderPass
        m_backend = nullptr;
    }

    void Framebuffer::SetSize(const Vec2i& newSize)
    {
        if (newSize.x <= 0 || newSize.y <= 0)
            return;
        if (newSize == Size && m_backend->IsValid())
            return;

        Size = newSize;

        if (Texture* colorTex = Color.Owned.get())
            colorTex->Resize(newSize);
        if (Texture* depthTex = Depth.Owned.get())
            depthTex->Resize(newSize);

        TWISTED_WARN("Vulkan Framebuffer::SetSize(): framebuffer recreation not yet implemented");
        Version++;
    }

    void Framebuffer::Bind()
    {
        // TODO: begin Vulkan render pass via command buffer
    }

    void Framebuffer::Unbind()
    {
        // TODO: end Vulkan render pass via command buffer
    }
}

#endif

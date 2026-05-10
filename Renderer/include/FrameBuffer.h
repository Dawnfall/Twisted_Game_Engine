#pragma once

#include "AppCore.h"

#include "Application/TObject.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Texture.h"
#include "Utils/WPtr.h"

namespace Twisted
{
    struct FramebufferBackend;

    struct TWISTED_API FramebufferAttachment
    {
        WPtr<Texture> Override; // If set, used for rendering and NOT owned/resized by the framebuffer.
        WPtr<Texture> Owned;    // Created and managed by the framebuffer when Override is not set.

        // Descriptor for the owned texture (format/usage/etc). Size is driven by Framebuffer::Size.
        TextureInfo OwnedDesc{};

        // Subresource selection
        uint32_t Mip = 0;
        uint32_t Layer = 0;

        WPtr<Texture> Effective() const { return Override ? Override : Owned; }
        bool Has() const { return (bool)Effective(); }
    };

    struct TWISTED_API Framebuffer : public TObject
    {
        Framebuffer(const std::string& name);
        ~Framebuffer();

        void OnCreate() override;
        void OnDestroy() override;

        void SetSize(const Vec2i& newSize); // resizes owned attachments only
        void Bind();
        void Unbind();

        // Override control — set an externally owned texture as the attachment
        void SetColorOverride(WPtr<Texture> tex) { Color.Override = tex; Version++; }
        void SetDepthOverride(WPtr<Texture> tex) { Depth.Override = tex; Version++; }
        void ClearColorOverride() { Color.Override = nullptr; Version++; }
        void ClearDepthOverride() { Depth.Override = nullptr; Version++; }

        // Owned attachment descriptors — applied on next OnCreate / SetSize
        void SetOwnedColorDesc(const TextureInfo& desc) { Color.OwnedDesc = desc; Version++; }
        void SetOwnedDepthDesc(const TextureInfo& desc) { Depth.OwnedDesc = desc; Version++; }

        WPtr<Texture> GetColor() const { return Color.Effective(); }
        WPtr<Texture> GetDepth() const { return Depth.Effective(); }

        Vec2i GetSize() const { return Size; }
        bool IsReady() const { return Size.x > 0 && Size.y > 0; }
        uint32_t GetVersion() const { return Version; }

        FramebufferBackend* GetBackend() { return m_backend.get(); }

    private:
        friend struct FramebufferBackend;

        URef<FramebufferBackend> m_backend;

        Vec2i Size{ 0, 0 };
        uint32_t Version = 0;

        FramebufferAttachment Color;
        FramebufferAttachment Depth;
    };

}

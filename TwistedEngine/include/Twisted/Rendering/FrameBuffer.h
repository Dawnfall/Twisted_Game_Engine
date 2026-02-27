#pragma once

#include "AppCore.h"

#include "Twisted/TObject.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Texture.h"
#include "Utils/WPtr.h"

namespace Twisted
{
    struct FramebufferBackend;

    struct TWISTED_API FramebufferAttachment
    {        
        WPtr<Texture> Override; // If Override is set, it is used for rendering and NOT owned/resized by the framebuffer.
        WPtr<Texture> Owned; // Owned texture used when Override is not set.

        // Descriptor for the owned texture (format/usage/etc). Size is driven by Framebuffer::Size.
        TextureInfo OwnedDesc{};

        // Subresource selection (optional for now)
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

        void SetSize(const Vec2i& newSize); // resizes/recreates owned attachments only

        // Attachment control
        void SetColorOverride(WPtr<Texture> tex) { Color.Override = tex; Version++; }
        void SetDepthOverride(WPtr<Texture> tex) { Depth.Override = tex; Version++; }
        void ClearColorOverride() { Color.Override->Clear(); Version++; }
        void ClearDepthOverride() { Depth.Override->Clear(); Version++; }

        void SetOwnedColorDesc(const TextureInfo& desc) { Color.OwnedDesc = desc; Version++; }
        void SetOwnedDepthDesc(const TextureInfo& desc) { Depth.OwnedDesc = desc; Version++; }

        WPtr<Texture> GetColor() const { return Color.Effective(); }
        WPtr<Texture> GetDepth() const { return Depth.Effective(); }

        Vec2i GetSize() const { return Size; }
        uint32_t GetVersion() const { return Version; }

        const FramebufferBackend* GetBackend() const { return m_backend; } // if you keep it exposed

    private:
        FramebufferBackend* m_backend = nullptr;

        Vec2i Size{ 0, 0 };
        uint32_t Version = 0;

        FramebufferAttachment Color;
        FramebufferAttachment Depth;
    };

}


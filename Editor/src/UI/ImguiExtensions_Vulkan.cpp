#include "UI/ImguiExtensions.h"

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>

#include "Texture.h"
#include "VulkanContext.h"
#include "RenderAPI.h"

#include <unordered_map>

namespace Im
{
    struct CachedDescriptor
    {
        uint32_t       version;
        VkDescriptorSet ds;
    };

    static std::unordered_map<const Twisted::Texture*, CachedDescriptor> s_texCache;

    ImTextureID GetImGuiTextureID(const Twisted::Texture* tex)
    {
        if (!tex || !tex->IsValid()) return 0;

        auto it = s_texCache.find(tex);
        if (it != s_texCache.end())
        {
            if (it->second.version == tex->Version)
                return static_cast<ImTextureID>(reinterpret_cast<uintptr_t>(it->second.ds));

            ImGui_ImplVulkan_RemoveTexture(it->second.ds);
            s_texCache.erase(it);
        }

        VkDescriptorSet ds = ImGui_ImplVulkan_AddTexture(
            tex->Sampler, tex->ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        s_texCache[tex] = { tex->Version, ds };
        return static_cast<ImTextureID>(reinterpret_cast<uintptr_t>(ds));
    }

    void Render()
    {
        ImGui::Render();
        auto& ctx = Twisted::VK::VulkanContext::Get();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                        ctx.CommandBuffers[ctx.CurrentFrame]);
    }

    void EndFrame()
    {
        ImGui::Render();
        if (Twisted::Render::IsFrameActive())
        {
            auto& ctx = Twisted::VK::VulkanContext::Get();
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                            ctx.CommandBuffers[ctx.CurrentFrame]);
            Twisted::Render::PresentFrame();
        }
    }
}

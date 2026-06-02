#include "RenderAPI.h"
#include "VulkanContext.h"
#include "FrameBuffer.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Data/RenderContext.h"
#include "Data/ShaderLayouts.h"
#include "Window.h"
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"
#include "Debug/Logger.h"

#include <cstring>

namespace Twisted::Render
{
    static uint32_t s_imageIndex        = 0;
    static bool     s_frameActive       = false;  // true while inside the swapchain render pass
    static bool     s_cmdBufferActive   = false;  // true while command buffer is open (after BeginFrame)
    static bool     s_cameraPassActive  = false;  // true between BeginCameraPass and EndCameraPass
    static bool     s_rebuildPending    = false;
    static int      s_cameraSlot        = 0;      // per-frame camera index (reset each BeginFrame)

    void Init() {}

    bool IsCmdBufferOpen() { return s_cmdBufferActive; }

    void CreateContext(Window* window)
    {
        VK::VulkanContext::Init(static_cast<GLFWwindow*>(window->GetRawPointer()));
    }

    void EnableDepthTest([[maybe_unused]] bool doTest)  {}
    void EnableStencilTest([[maybe_unused]] bool doTest) {}

    bool IsFrameActive() { return s_frameActive; }

    // -----------------------------------------------------------------------
    // BeginFrame — acquire swapchain image, open command buffer (no render pass yet).
    // Call RenderOffscreen for each offscreen target, then BeginSwapchainPass.
    // -----------------------------------------------------------------------
    void BeginFrame()
    {
        s_cmdBufferActive  = false;
        s_frameActive      = false;
        s_cameraPassActive = false;
        s_cameraSlot       = 0;

        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady()) return;

        // Skip rendering while the window is minimized (framebuffer = 0x0).
        // vkCreateSwapchainKHR rejects a 0x0 extent, so we must not rebuild
        // the swapchain until the window is restored.
        if (ctx.GlfwWindow)
        {
            int w = 0, h = 0;
            glfwGetFramebufferSize(ctx.GlfwWindow, &w, &h);
            if (w == 0 || h == 0) return;
        }

        const uint32_t frame = ctx.CurrentFrame;

        vkWaitForFences(ctx.Device, 1, &ctx.InFlightFences[frame], VK_TRUE, UINT64_MAX);

        // Rebuild if a previous present returned SUBOPTIMAL (e.g. after maximize).
        if (s_rebuildPending)
        {
            s_rebuildPending = false;
            ctx.RebuildSwapchain(ctx.SwapchainExtent.width, ctx.SwapchainExtent.height);
            return;
        }

        VkResult result = vkAcquireNextImageKHR(ctx.Device, ctx.Swapchain, UINT64_MAX,
                                                ctx.ImageAvailableSemaphores[frame],
                                                VK_NULL_HANDLE, &s_imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            // Leave fence signaled so next BeginFrame doesn't deadlock on vkWaitForFences
            ctx.RebuildSwapchain(ctx.SwapchainExtent.width, ctx.SwapchainExtent.height);
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            TWISTED_ERROR("[Vulkan] Failed to acquire swapchain image");
            return;
        }

        vkResetFences(ctx.Device, 1, &ctx.InFlightFences[frame]);

        VkCommandBuffer cmd = ctx.CommandBuffers[frame];
        vkResetCommandBuffer(cmd, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(cmd, &beginInfo);

        s_cmdBufferActive = true;

        // NOTE: swapchain render pass moved to BeginSwapchainPass()
        // std::array<VkClearValue, 2> clearValues{};
        // clearValues[0].color        = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
        // clearValues[1].depthStencil = { 1.0f, 0 };
        //
        // VkRenderPassBeginInfo rpBegin{};
        // rpBegin.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        // rpBegin.renderPass        = ctx.RenderPass;
        // rpBegin.framebuffer       = ctx.SwapchainFramebuffers[s_imageIndex];
        // rpBegin.renderArea.offset = { 0, 0 };
        // rpBegin.renderArea.extent = ctx.SwapchainExtent;
        // rpBegin.clearValueCount   = static_cast<uint32_t>(clearValues.size());
        // rpBegin.pClearValues      = clearValues.data();
        //
        // vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
        //
        // VkViewport viewport{};
        // viewport.width    = static_cast<float>(ctx.SwapchainExtent.width);
        // viewport.height   = static_cast<float>(ctx.SwapchainExtent.height);
        // viewport.maxDepth = 1.0f;
        // vkCmdSetViewport(cmd, 0, 1, &viewport);
        //
        // VkRect2D scissor{ { 0, 0 }, ctx.SwapchainExtent };
        // vkCmdSetScissor(cmd, 0, 1, &scissor);
        //
        // s_frameActive = true;
    }

    // -----------------------------------------------------------------------
    // BeginSwapchainPass — begin the swapchain render pass + set viewport/scissor.
    // Must be called after BeginFrame and all RenderOffscreen calls.
    // -----------------------------------------------------------------------
    void BeginSwapchainPass()
    {
        if (!s_cmdBufferActive || s_frameActive) return;

        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady()) return;

        VkCommandBuffer cmd = ctx.CommandBuffers[ctx.CurrentFrame];

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color        = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkRenderPassBeginInfo rpBegin{};
        rpBegin.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegin.renderPass        = ctx.RenderPass;
        rpBegin.framebuffer       = ctx.SwapchainFramebuffers[s_imageIndex];
        rpBegin.renderArea.offset = { 0, 0 };
        rpBegin.renderArea.extent = ctx.SwapchainExtent;
        rpBegin.clearValueCount   = static_cast<uint32_t>(clearValues.size());
        rpBegin.pClearValues      = clearValues.data();

        vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.width    = static_cast<float>(ctx.SwapchainExtent.width);
        viewport.height   = static_cast<float>(ctx.SwapchainExtent.height);
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor{ { 0, 0 }, ctx.SwapchainExtent };
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        s_frameActive = true;
    }

    // -----------------------------------------------------------------------
    // RenderOffscreen — clear fb using its own render pass (no draws, just the clear).
    // Must be called after BeginFrame and before BeginSwapchainPass.
    // -----------------------------------------------------------------------
    void RenderOffscreen(Framebuffer* fb, Vec4f clearColor)
    {
        if (!fb || !fb->IsReady() || !fb->IsValid()) return;
        if (!s_cmdBufferActive || s_frameActive) return;

        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady()) return;

        VkCommandBuffer cmd = ctx.CommandBuffers[ctx.CurrentFrame];

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color        = {{ clearColor.r, clearColor.g, clearColor.b, clearColor.a }};
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkRenderPassBeginInfo rpBegin{};
        rpBegin.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegin.renderPass        = fb->VkRP;
        rpBegin.framebuffer       = fb->VkFB;
        rpBegin.renderArea.offset = { 0, 0 };
        rpBegin.renderArea.extent = { static_cast<uint32_t>(fb->GetSize().x),
                                      static_cast<uint32_t>(fb->GetSize().y) };
        rpBegin.clearValueCount   = static_cast<uint32_t>(clearValues.size());
        rpBegin.pClearValues      = clearValues.data();

        vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.width    = static_cast<float>(fb->GetSize().x);
        viewport.height   = static_cast<float>(fb->GetSize().y);
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor{ { 0, 0 }, { static_cast<uint32_t>(fb->GetSize().x),
                                       static_cast<uint32_t>(fb->GetSize().y) } };
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        vkCmdEndRenderPass(cmd); // clear-only pass — no draws recorded
    }

    // -----------------------------------------------------------------------
    // BeginCameraPass — open the camera FB's render pass (clear + keeps it open for draws).
    // Follow with ForwardRenderCamera then EndCameraPass.
    // -----------------------------------------------------------------------
    void BeginCameraPass(Framebuffer* fb, Vec4f clearColor)
    {
        if (!fb || !fb->IsReady() || !fb->IsValid()) return;
        if (!s_cmdBufferActive || s_frameActive || s_cameraPassActive) return;

        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady()) return;

        VkCommandBuffer cmd = ctx.CommandBuffers[ctx.CurrentFrame];

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color        = {{ clearColor.r, clearColor.g, clearColor.b, clearColor.a }};
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkRenderPassBeginInfo rpBegin{};
        rpBegin.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegin.renderPass        = fb->VkRP;
        rpBegin.framebuffer       = fb->VkFB;
        rpBegin.renderArea.offset = { 0, 0 };
        rpBegin.renderArea.extent = { static_cast<uint32_t>(fb->GetSize().x),
                                      static_cast<uint32_t>(fb->GetSize().y) };
        rpBegin.clearValueCount   = static_cast<uint32_t>(clearValues.size());
        rpBegin.pClearValues      = clearValues.data();

        vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.width    = static_cast<float>(fb->GetSize().x);
        viewport.height   = static_cast<float>(fb->GetSize().y);
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor{ { 0, 0 }, { static_cast<uint32_t>(fb->GetSize().x),
                                       static_cast<uint32_t>(fb->GetSize().y) } };
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        s_cameraPassActive = true;
    }

    // -----------------------------------------------------------------------
    // EndCameraPass — close the camera FB render pass (transitions to SHADER_READ_ONLY_OPTIMAL).
    // -----------------------------------------------------------------------
    void EndCameraPass()
    {
        if (!s_cameraPassActive) return;
        auto& ctx = VK::VulkanContext::Get();
        vkCmdEndRenderPass(ctx.CommandBuffers[ctx.CurrentFrame]);
        s_cameraPassActive = false;
    }

    // -----------------------------------------------------------------------
    // ForwardRenderCamera — draw all models for one camera into the active camera pass.
    // Must be called between BeginCameraPass and EndCameraPass.
    // -----------------------------------------------------------------------
    void ForwardRenderCamera(const RenderContext& context, const CameraData& camData)
    {
        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady() || !s_cameraPassActive) return;

        const uint32_t frame = ctx.CurrentFrame;
        VkCommandBuffer cmd  = ctx.CommandBuffers[frame];

        // Claim a per-camera slot so each camera writes to its own MVP UBO region.
        // s_cameraSlot is reset to 0 in BeginFrame and wraps at MaxCamerasPerFrame.
        const int      slot    = s_cameraSlot < VK::MaxCamerasPerFrame ? s_cameraSlot++ : VK::MaxCamerasPerFrame - 1;
        const int      descIdx = static_cast<int>(frame) * VK::MaxCamerasPerFrame + slot;

        if (descIdx >= static_cast<int>(ctx.GlobalDescSets.size()) ||
            ctx.GlobalDescSets[descIdx] == VK_NULL_HANDLE)
            return;

        // Update lights UBO (shared across all cameras this frame — last write wins, that's fine)
        if (ctx.LightsUBOMapped[frame])
        {
            ShaderLightsBuffer lights{};
            lights.dirLightCount   = static_cast<int>(context.lightData.dirLights.size());
            lights.pointLightCount = static_cast<int>(context.lightData.pointLights.size());
            lights.spotLightCount  = static_cast<int>(context.lightData.spotLights.size());
            lights.ambientLight    = context.lightData.ambient;

            for (int i = 0; i < lights.dirLightCount; ++i)
                lights.dirLights[i] = context.lightData.dirLights[i];
            for (int i = 0; i < lights.pointLightCount; ++i)
                lights.pointLights[i] = context.lightData.pointLights[i];
            for (int i = 0; i < lights.spotLightCount; ++i)
                lights.spotLights[i] = context.lightData.spotLights[i];

            memcpy(ctx.LightsUBOMapped[frame], &lights, sizeof(lights));
        }

        // Write VP into this camera's dedicated slot — never overwrites another camera's data.
        if (ctx.MvpUBOMapped[frame])
        {
            ShaderMVPBuffer vp{};
            vp.ViewMatrix       = camData.viewMatrix;
            vp.ProjectionMatrix = camData.projectionMatrix;
            vp.CameraPosition   = Vec4f(camData.cameraPosition, 0.0f);
            char* dst = static_cast<char*>(ctx.MvpUBOMapped[frame]) + slot * ctx.MvpSlotStride;
            memcpy(dst, &vp, sizeof(vp));
        }

        for (const auto& modelData : context.modelDatas)
        {
            if (!modelData.mesh || !modelData.material) continue;
            Shader* shader = modelData.material->GetShader();
            if (!shader || !shader->IsValid()) continue;

            const Mesh* mesh = modelData.mesh;
            if (!mesh || !mesh->IsValid()) continue;

            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->Pipeline);

            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    shader->PipelineLayout, 0, 1,
                                    &ctx.GlobalDescSets[descIdx], 0, nullptr);

            if (!modelData.material->TextureDescSets.empty())
                vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        shader->PipelineLayout, 1, 1,
                                        &modelData.material->TextureDescSets[frame], 0, nullptr);

            ShaderPushConstants pc{};
            pc.ModelMatrix = modelData.modelMatrix;
            pc.Color       = modelData.material->PushColor;
            vkCmdPushConstants(cmd, shader->PipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0, sizeof(ShaderPushConstants), &pc);

            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &mesh->VertexBuffer, &offset);
            vkCmdBindIndexBuffer(cmd, mesh->IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(cmd,
                             static_cast<uint32_t>(modelData.mesh->GetIndexCount()),
                             1, 0, 0, 0);
        }
    }

    // -----------------------------------------------------------------------
    // PrepareRender — flush material uniforms before the command buffer opens
    // -----------------------------------------------------------------------
    void PrepareRender(const RenderContext& context)
    {
        for (const auto& modelData : context.modelDatas)
            if (modelData.material)
                modelData.material->ApplyUniforms();
    }

    // -----------------------------------------------------------------------
    // ForwardRender — record scene draw commands into the open command buffer
    // -----------------------------------------------------------------------
    void ForwardRender(const RenderContext& context)
    {
        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady() || !s_frameActive) return;

        const uint32_t frame = ctx.CurrentFrame;
        VkCommandBuffer cmd  = ctx.CommandBuffers[frame];

        // Update lights UBO
        if (!context.camDatas.empty() && ctx.LightsUBOMapped[frame])
        {
            ShaderLightsBuffer lights{};
            lights.dirLightCount   = static_cast<int>(context.lightData.dirLights.size());
            lights.pointLightCount = static_cast<int>(context.lightData.pointLights.size());
            lights.spotLightCount  = static_cast<int>(context.lightData.spotLights.size());
            lights.ambientLight    = context.lightData.ambient;

            for (int i = 0; i < lights.dirLightCount; ++i)
                lights.dirLights[i] = context.lightData.dirLights[i];
            for (int i = 0; i < lights.pointLightCount; ++i)
                lights.pointLights[i] = context.lightData.pointLights[i];
            for (int i = 0; i < lights.spotLightCount; ++i)
                lights.spotLights[i] = context.lightData.spotLights[i];

            memcpy(ctx.LightsUBOMapped[frame], &lights, sizeof(lights));
        }

        // Recording — descriptor sets must already be up-to-date (call PrepareRender first).
        for (const auto& camData : context.camDatas)
        {
            // Write VP data once per camera — model matrix is a per-draw push constant.
            if (ctx.MvpUBOMapped[frame])
            {
                ShaderMVPBuffer vp{};
                vp.ViewMatrix       = camData.viewMatrix;
                vp.ProjectionMatrix = camData.projectionMatrix;
                vp.CameraPosition   = Vec4f(camData.cameraPosition, 0.0f);
                memcpy(ctx.MvpUBOMapped[frame], &vp, sizeof(vp));
            }

            for (const auto& modelData : context.modelDatas)
            {
                if (!modelData.mesh || !modelData.material) continue;
                Shader* shader = modelData.material->GetShader();
                if (!shader || !shader->IsValid()) continue;

                const Mesh* mesh = modelData.mesh;
                if (!mesh || !mesh->IsValid()) continue;

                vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->Pipeline);

                vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        shader->PipelineLayout, 0, 1,
                                        &ctx.GlobalDescSets[frame], 0, nullptr);

                if (!modelData.material->TextureDescSets.empty())
                    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                            shader->PipelineLayout, 1, 1,
                                            &modelData.material->TextureDescSets[frame], 0, nullptr);

                ShaderPushConstants pc;
                pc.ModelMatrix = modelData.modelMatrix;
                pc.Color       = modelData.material->PushColor;
                vkCmdPushConstants(cmd, shader->PipelineLayout,
                                   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                                   0, sizeof(ShaderPushConstants), &pc);

                VkDeviceSize offset = 0;
                vkCmdBindVertexBuffers(cmd, 0, 1, &mesh->VertexBuffer, &offset);
                vkCmdBindIndexBuffer(cmd, mesh->IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

                vkCmdDrawIndexed(cmd,
                                 static_cast<uint32_t>(modelData.mesh->GetIndexCount()),
                                 1, 0, 0, 0);
            }
        }
    }

    // -----------------------------------------------------------------------
    // PresentFrame — end render pass, submit, present, advance frame index
    // -----------------------------------------------------------------------
    void PresentFrame()
    {
        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady() || !s_frameActive) return;
        s_frameActive     = false;
        s_cmdBufferActive = false;

        const uint32_t frame  = ctx.CurrentFrame;
        VkCommandBuffer cmd   = ctx.CommandBuffers[frame];

        vkCmdEndRenderPass(cmd);
        vkEndCommandBuffer(cmd);

        VkSemaphore          waitSems[]   = { ctx.ImageAvailableSemaphores[frame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore          signalSems[] = { ctx.RenderFinishedSemaphores[s_imageIndex] };

        VkSubmitInfo submit{};
        submit.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.waitSemaphoreCount   = 1;
        submit.pWaitSemaphores      = waitSems;
        submit.pWaitDstStageMask    = waitStages;
        submit.commandBufferCount   = 1;
        submit.pCommandBuffers      = &cmd;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores    = signalSems;

        if (vkQueueSubmit(ctx.GraphicsQueue, 1, &submit, ctx.InFlightFences[frame]) != VK_SUCCESS)
            TWISTED_ERROR("[Vulkan] Failed to submit draw command buffer");

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = signalSems;
        presentInfo.swapchainCount     = 1;
        presentInfo.pSwapchains        = &ctx.Swapchain;
        presentInfo.pImageIndices      = &s_imageIndex;

        VkResult result = vkQueuePresentKHR(ctx.PresentQueue, &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            TWISTED_WARN("[Vulkan] Swapchain suboptimal — rebuild on next frame");
            s_rebuildPending = true;
        }

        ctx.CurrentFrame = (ctx.CurrentFrame + 1) % VK::FramesInFlight;
    }

    void Shutdown()
    {
        VK::VulkanContext::Get().Shutdown();
    }
}

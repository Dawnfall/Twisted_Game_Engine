#ifdef TWISTED_BACKEND_VULKAN

#include "Twisted/Rendering/RenderAPI.h"
#include "Twisted/Rendering/Vulkan/VulkanContext.h"
#include "Twisted/Rendering/Vulkan/Shader_Vulkan.h"
#include "Twisted/Rendering/Vulkan/Mesh_Vulkan.h"
#include "Twisted/Rendering/Data/RenderContext.h"
#include "Twisted/Rendering/Data/ShaderLayouts.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/Rendering/Mesh.h"
#include "Debug/Logger.h"

#include <cstring>

namespace Twisted::Render
{
    void Init()
    {
        // Deferred to InitSurface (needs window handle).
    }

    void InitSurface(void* nativeWindowHandle)
    {
        VK::VulkanContext::Get().InitSurface(nativeWindowHandle);
    }

    void EnableDepthTest([[maybe_unused]] bool doTest)  {}
    void EnableStencilTest([[maybe_unused]] bool doTest) {}

    // -----------------------------------------------------------------------
    // ForwardRender — frame loop
    // -----------------------------------------------------------------------
    void ForwardRender(const RenderContext& context)
    {
        auto& ctx = VK::VulkanContext::Get();
        if (!ctx.IsReady()) return;

        const uint32_t frame = ctx.CurrentFrame;

        // ---- Wait for previous use of this frame slot ----
        vkWaitForFences(ctx.Device, 1, &ctx.InFlightFences[frame], VK_TRUE, UINT64_MAX);

        // ---- Acquire next swapchain image ----
        uint32_t imageIndex = 0;
        VkResult result = vkAcquireNextImageKHR(ctx.Device, ctx.Swapchain, UINT64_MAX,
                                                ctx.ImageAvailableSemaphores[frame],
                                                VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            TWISTED_WARN("[Vulkan] Swapchain out of date — rebuild needed");
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            TWISTED_ERROR("[Vulkan] Failed to acquire swapchain image");
            return;
        }

        vkResetFences(ctx.Device, 1, &ctx.InFlightFences[frame]);

        // ---- Update global UBOs (MVP written per-draw below; Lights written once) ----
        if (!context.camDatas.empty() && ctx.LightsUBOMapped[frame])
        {
            // Build lights buffer and upload
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

        // ---- Record command buffer ----
        VkCommandBuffer cmd = ctx.CommandBuffers[frame];
        vkResetCommandBuffer(cmd, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(cmd, &beginInfo);

        // Use the first camera's clear color, or black default
        Vec4f clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        if (!context.camDatas.empty())
            clearColor = context.camDatas[0].clearParams.clearColor;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color        = {{ clearColor.r, clearColor.g, clearColor.b, clearColor.a }};
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkRenderPassBeginInfo rpBegin{};
        rpBegin.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegin.renderPass        = ctx.RenderPass;
        rpBegin.framebuffer       = ctx.SwapchainFramebuffers[imageIndex];
        rpBegin.renderArea.offset = { 0, 0 };
        rpBegin.renderArea.extent = ctx.SwapchainExtent;
        rpBegin.clearValueCount   = static_cast<uint32_t>(clearValues.size());
        rpBegin.pClearValues      = clearValues.data();

        vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

        // Dynamic viewport + scissor
        VkViewport viewport{};
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = static_cast<float>(ctx.SwapchainExtent.width);
        viewport.height   = static_cast<float>(ctx.SwapchainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor{ { 0, 0 }, ctx.SwapchainExtent };
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        // ---- Draw calls — one camera at a time ----
        for (const auto& camData : context.camDatas)
        {
            for (const auto& modelData : context.modelDatas)
            {
                if (!modelData.mesh || !modelData.material) continue;
                auto* backend = modelData.material->GetShader()
                                ? modelData.material->GetShader()->GetBackend()
                                : nullptr;
                if (!backend || !backend->IsValid()) continue;

                const MeshBackend* meshBk = modelData.mesh->GetBackend();
                if (!meshBk || !meshBk->IsValid()) continue;

                // Upload MVP for this draw
                if (ctx.MvpUBOMapped[frame])
                {
                    ShaderMVPBuffer mvp{};
                    mvp.ModelMatrix      = modelData.modelMatrix;
                    mvp.ViewMatrix       = camData.viewMatrix;
                    mvp.ProjectionMatrix = camData.projectionMatrix;
                    mvp.CameraPosition   = Vec4f(camData.cameraPosition, 0.0f);
                    memcpy(ctx.MvpUBOMapped[frame], &mvp, sizeof(mvp));
                }

                vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, backend->Pipeline);

                // Set 0: global MVP + Lights
                vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        backend->PipelineLayout, 0, 1,
                                        &ctx.GlobalDescSets[frame], 0, nullptr);

                // Set 1: per-shader textures
                if (!backend->TextureDescSets.empty())
                    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                            backend->PipelineLayout, 1, 1,
                                            &backend->TextureDescSets[frame], 0, nullptr);

                VkDeviceSize offset = 0;
                vkCmdBindVertexBuffers(cmd, 0, 1, &meshBk->VertexBuffer, &offset);
                vkCmdBindIndexBuffer(cmd, meshBk->IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

                vkCmdDrawIndexed(cmd,
                                 static_cast<uint32_t>(modelData.mesh->GetIndexCount()),
                                 1, 0, 0, 0);
            }
        }

        vkCmdEndRenderPass(cmd);
        vkEndCommandBuffer(cmd);

        // ---- Submit ----
        VkSemaphore          waitSems[]   = { ctx.ImageAvailableSemaphores[frame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore          signalSems[] = { ctx.RenderFinishedSemaphores[frame] };

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

        // ---- Present ----
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = signalSems;
        presentInfo.swapchainCount     = 1;
        presentInfo.pSwapchains        = &ctx.Swapchain;
        presentInfo.pImageIndices      = &imageIndex;

        result = vkQueuePresentKHR(ctx.PresentQueue, &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            TWISTED_WARN("[Vulkan] Swapchain suboptimal — rebuild on next frame");

        ctx.CurrentFrame = (ctx.CurrentFrame + 1) % VK::FramesInFlight;
    }
}

#endif

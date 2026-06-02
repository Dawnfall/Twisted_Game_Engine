#pragma once

#include "AppCore.h"
#include "Data/RenderContext.h"
#include "Utils/GlmUtils.h"
#include "Window.h"

namespace Twisted { struct Framebuffer; }

namespace Twisted::Render
{
	void TWISTED_API Init();

	void TWISTED_API CreateContext(Window* window);

	void TWISTED_API EnableDepthTest(bool doTest);

	void TWISTED_API EnableStencilTest(bool doTest);

	// PrepareRender: flush material uniforms (descriptor set writes) for a context.
	// Must be called for all contexts BEFORE BeginFrame — updating a descriptor set
	// that is bound to a recording command buffer invalidates it.
	void TWISTED_API PrepareRender(const RenderContext& renderContext);

	// BeginFrame: acquire swapchain image, open command buffer (no render pass yet).
	// Call BeginCameraPass/ForwardRenderCamera/EndCameraPass per camera, then BeginSwapchainPass.
	void TWISTED_API BeginFrame();

	// BeginSwapchainPass: begin the swapchain render pass + set viewport/scissor.
	// Must be called after BeginFrame and all camera passes.
	void TWISTED_API BeginSwapchainPass();

	// BeginCameraPass: begin fb's render pass (clear + open), set viewport/scissor.
	// Follow with ForwardRenderCamera then EndCameraPass.
	void TWISTED_API BeginCameraPass(Framebuffer* fb, Vec4f clearColor = Vec4f(0.1f, 0.1f, 0.1f, 1.0f));

	// EndCameraPass: end the current camera fb render pass (transitions to SHADER_READ_ONLY_OPTIMAL).
	void TWISTED_API EndCameraPass();

	// ForwardRenderCamera: draw all models for one camera into the active camera pass.
	// Must be called between BeginCameraPass and EndCameraPass.
	void TWISTED_API ForwardRenderCamera(const RenderContext& renderContext, const CameraData& camData);

	// RenderOffscreen: clear fb (clear-only, no draws). Kept for backward compat.
	void TWISTED_API RenderOffscreen(Framebuffer* fb, Vec4f clearColor = Vec4f(0.1f, 0.1f, 0.1f, 1.0f));

	// IsFrameActive: true between BeginSwapchainPass and PresentFrame (swapchain render pass open).
	bool TWISTED_API IsFrameActive();

	// IsCmdBufferOpen: true between BeginFrame and PresentFrame (command buffer is recording).
	bool TWISTED_API IsCmdBufferOpen();

	// ForwardRender: record scene draw commands into the open command buffer.
	// Call PrepareRender first to ensure descriptor sets are up-to-date.
	void TWISTED_API ForwardRender(const RenderContext& renderContext);

	// PresentFrame: end render pass, submit, present, advance frame index
	void TWISTED_API PresentFrame();

	// Shutdown: destroy all Vulkan resources — call before destroying the window
	void TWISTED_API Shutdown();
}

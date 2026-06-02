#include "RenderService.h"
#include "RenderAPI.h"
#include "FrameBuffer.h"
#include "Input.h"
#include "Application/Application.h"
#include "VulkanContext.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>

namespace Twisted
{
	RenderService::RenderService(Application* app, int priority) : Service(app, priority) {}

	void RenderService::OnInit() {}
	void RenderService::OnTerminate() {}

	void RenderService::PollEvents()
	{
		Input::GetInstance().BeginFrame();
		glfwPollEvents();
	}

	Window* RenderService::CreateNewWindow(const std::string& title, const Vec2i& size, const Vec2i& position)
	{
		m_window = std::make_unique<Window>(this, title, size, position);
		Twisted::Render::CreateContext(m_window.get());
		WindowCreatedEvent.Invoke(m_window.get());
		return m_window.get();
	}

	void RenderService::DestroyWindow(Window* window)
	{
		if (window && m_window && window == m_window.get())
			m_window = nullptr;
	}

	void RenderService::DispatchEvent(const KeyEvent& e)
	{
		Input::GetInstance().UpdateKey(e.key, e.state);
	}
	void RenderService::DispatchEvent(const MouseButtonEvent& e)
	{
		Input::GetInstance().UpdateMouseButton(e.button, e.state);
	}
	void RenderService::DispatchEvent(const MouseMoveEvent& e)
	{
		Input::GetInstance().UpdateMousePosition(e.position.x, e.position.y);
	}
	void RenderService::DispatchEvent(const MouseWheelEvent& e)
	{
		Input::GetInstance().UpdateMouseWheel(e.delta);
	}
	void RenderService::DispatchEvent(const WindowCloseEvent&)
	{
		if (m_window)
			m_window->CloseWindowEvent.Invoke();
	}
	void RenderService::DispatchEvent(const WindowResizeEvent& e)
	{
		if (m_window)
			m_window->WindowResizeEvent.Invoke(e.size);
	}
	void RenderService::DispatchEvent(const WindowFocusEvent& e)
	{
		if (!e.gained)
			Input::GetInstance().ClearAllInput();
	}
	void RenderService::DispatchEvent(const FilesDroppedEvent& e)
	{
		if (m_window)
			m_window->FilesDroppedEvent.Invoke(e.paths);
	}

	void RenderService::SubmitContext(RenderContext context)
	{
		m_queue.push(std::move(context));
	}

	void RenderService::WaitIdle() const
	{
		vkDeviceWaitIdle(VK::VulkanContext::Get().Device);
	}

	void RenderService::Render()
	{
		if (!m_window)
			return;

		std::vector<RenderContext> contexts;
		while (!m_queue.empty())
		{
			contexts.push_back(std::move(m_queue.front()));
			m_queue.pop();
		}

		// BeginFrame waits for the current frame's fence so descriptor sets are safe to update.
		Twisted::Render::BeginFrame();

		// Flush material descriptor sets now that the fence guarantees they're not in-flight.
		for (const auto& ctx : contexts)
			Twisted::Render::PrepareRender(ctx);

		// Render each camera's scene into its offscreen framebuffer.
		// EndCameraPass transitions the texture to SHADER_READ_ONLY_OPTIMAL so ImGui can sample it.
		for (const auto& ctx : contexts)
			for (const auto& camData : ctx.camDatas)
				if (camData.framebuffer && camData.framebuffer->IsValid())
				{
					Twisted::Render::BeginCameraPass(camData.framebuffer, camData.clearParams.clearColor);
					Twisted::Render::ForwardRenderCamera(ctx, camData);
					Twisted::Render::EndCameraPass();
				}

		Twisted::Render::BeginSwapchainPass();

		// Old single-pass forward render (kept for reference — now replaced by per-camera passes above):
		//for (const auto& ctx : contexts)
		//	Twisted::Render::PrepareRender(ctx);
		//for (auto& ctx : contexts)
		//	Twisted::Render::ForwardRender(ctx);
	}
}

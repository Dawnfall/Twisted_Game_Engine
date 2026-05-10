#include "RenderService.h"
#include "RenderAPI.h"
#include "WindowsService.h"
#include "Window.h"
#include "Application/Application.h"

namespace Twisted
{
	RenderService::RenderService(Application* app,int priority) :Service(app, priority) {};

	void RenderService::OnInit()
	{
		auto* windowsService = m_app->GetService<WindowsService>();
		windowsService->WindowCreatedEvent.AddListener([this](Window* window)
			{
				m_window = window;
#ifdef TWISTED_BACKEND_OPENGL
				window->AcquireGLContext();
#elif defined(TWISTED_BACKEND_VULKAN)
				Twisted::Render::InitSurface(window->GetRawPointer());
#endif
			});
	}

	void RenderService::OnTerminate()
	{

	}

	void RenderService::SubmitContext(RenderContext context)
	{
		m_queue.push(std::move(context));
	}

	void RenderService::Render()
	{
		while (!m_queue.empty() && m_window)
		{
			RenderContext ctx = std::move(m_queue.front());
			m_queue.pop();
			Twisted::Render::ForwardRender(ctx);
		}
	}
}

#include "Twisted/Rendering/RenderService.h"
#include "Twisted/Rendering/RenderAPI.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/Windowing/Window.h"
#include "Twisted/Application/Application.h"

namespace Twisted
{
	RenderService::RenderService(Application* app,int priority) :Service(app, priority) {};

	void RenderService::OnInit()
	{
		auto* windowsService = m_app->GetService<WindowsService>();
		windowsService->WindowCreatedEvent.AddListener([this](Window* window)
			{
				m_window = window;
				window->AcquireGLContext();
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
		if (!m_queue.empty() && m_window)
		{
			RenderContext ctx = std::move(m_queue.front());
			m_queue.pop();
			Twisted::Render::ForwardRender(ctx);
		}

	}
}

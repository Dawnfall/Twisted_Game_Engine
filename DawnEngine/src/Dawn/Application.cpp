#include "Application.h"

#include "Debug/Logger.h"
#include "Dawn/Game/World.h"

#include <memory>

namespace Dawn
{
	Application::Application()
	{

	}

	Application::~Application()
	{
		Window::Terminate(); //TODO: move to render core
	}

	void Application::Init()
	{
		Logger::Init();
		m_renderCore.Init();
		m_gameCore.Init();

	}

	void Application::Run()
	{
		bool shouldClose = false;
		while(IsRunning())
		{
			m_gameCore.Update();
			m_renderCore.Update();
		}
	}

	bool Application::IsRunning()
	{
		if (m_renderCore.GetWindow()->IsClosing())
			return false;
		return true;
	}
	//!m_window->ShouldClose()
}
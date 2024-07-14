#include "Application.h"

#include "Debug/Logger.h"
#include "Dawn/Game/World.h"
#include "Dawn/Rendering/RenderCore.h"
#include "../StartParams.h"
#include <memory>

namespace Dawn
{
	Application::Application()
	{
	}

	Application::~Application()
	{
		RenderCore::Terminate();
	}

	void Application::Start(const StartParams& startParams)
	{
		if (IsRunning())
		{
			DAWN_WARN("Cannot start application; Application already running!");
			return;
		}

		Init();

		OnStart();
	
		Run();
	}
	void Application::Init() 
	{
		if (Logger::Init() &&
			RenderCore::Init())
		{
			m_isValid = true;
			DAWN_INFO("Application Init Success!");
		}
		else
		{
			DAWN_ERROR("Application Init failure");
		}
	}
	void Application::Run()
	{
		m_isRunning = true;
		while (IsRunning())
		{
			WindowManager.UpdateWindow();
			OnRun();
		}
	}

	void Application::OnStart()
	{
		WindowManager.CreateNewWindow(WindowData());
		WindowManager.closeWindowEvent.AddListener([this]() { Stop(); });
	}
	void Application::OnRun()
	{

	}
}
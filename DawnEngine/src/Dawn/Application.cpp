#include "Application.h"

#include "Debug/Logger.h"
#include "Dawn/GameCore.h"
#include "Dawn/RenderCore.h"
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

	void Application::Start(RuntimeBase& runtime)
	{
		if (IsRunning())
		{
			DAWN_WARN("Cannot start application; Application already running!");
			return;
		}
		m_runtime = &runtime;

		Init();
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

		m_runtime->OnInit(*this);

		ResourceManager.CompileShaders();
	}

	void Application::BeforeStart()
	{
		m_runtime->OnBeforeRun(*this);
	}

	void Application::Run()
	{
		m_isRunning = true;
		while (IsRunning())
		{
			m_runtime->OnRun(*this);
			RenderCore::ClearWindows(WindowManager.GetWindows(),Collections::Color::blue);

			//TODO:... render here
			
			WindowManager.UpdateWindows();			
			//Systems.UpdateSystems();
		}
	}
}
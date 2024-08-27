#include "Application.h"

#include "Debug/Logger.h"
#include "Twisted/GameCore.h"
#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include <memory>

namespace Twisted
{
	Application::Application()
	{
	}
	Application::~Application()
	{
		Render_OpenGL::Terminate();
	}

	void Application::Start(RuntimeBase& runtime)
	{
		if (IsRunning())
		{
			TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}
		m_runtime = &runtime;

		Init();
		Run();
	}

	void Application::Init()
	{
		TimeManager.Init();

		if (Logger::Init() &&
			Render_OpenGL::InitGLFW())
		{
			m_isValid = true;
			TWISTED_INFO("Application Init Success!");
		}
		else
		{
			TWISTED_ERROR("Application Init failure");
		}

		m_runtime->OnInit(*this);
	}

	void Application::Run()
	{
		m_isRunning = true;

		m_runtime->OnBeforeRun(*this);
		while (IsRunning())
		{
			TimeManager.UpdateClocks();

			if (TimeManager.IsNextFrame())
			{
				TimeManager.IncreaseFrameCount();
				FrameUpdate();
				TimeManager.ResetFrameTime();
			}
		}
	}

	void Application::FrameUpdate()
	{
		m_runtime->OnRun(*this);

		for (auto& window : WindowManager.GetWindows())
		{
			glfwMakeContextCurrent(window->Pointer);
			Render_OpenGL::ClearWindow(Collections::Color::blue);
		}

		Game.Systems.UpdateSystems(*this);
		WindowManager.UpdateWindows();
	}
}
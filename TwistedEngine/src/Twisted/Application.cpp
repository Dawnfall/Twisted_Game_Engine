#include "pch.h"
#include "Application.h"

#include "Debug/Logger.h"
#include "Twisted/GameCore.h"
#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Game/Systems/RenderSystem.h"
#include "Editor/EditorCore.h"

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

		Game.Systems.AddSystem<Twisted::RenderSystem>();
		WindowManager.CreateNewWindow(m_runtime->Params.window1Title, m_runtime->Params.monitorWidth, m_runtime->Params.monitorHeight);

		//EDITOR_INIT(*this);

		m_runtime->OnInit(*this);
	}

	void Application::Run()
	{
		m_isRunning = true;

		m_runtime->OnBeforeRun(*this);
		while (IsRunning())
		{
			//EDITOR_UPDATE(*this);
			TimeManager.UpdateClocks();

			if (TimeManager.IsNextFrame())
			{
				FrameUpdate();
			}
		}
		Terminate();
	}

	void Application::FrameUpdate()
	{
		TimeManager.IncreaseFrameCount();

		m_runtime->OnRun(*this);

		glfwMakeContextCurrent(WindowManager.GetWindow()->Pointer);
		Render_OpenGL::ClearWindow(Collections::Color::blue);

		Game.Systems.UpdateSystems(*this);
		WindowManager.UpdateWindows();

		TimeManager.ResetFrameTime();
	}

	void Application::Terminate()
	{
		//EDITOR_TERMINATE(*this);
	}
}
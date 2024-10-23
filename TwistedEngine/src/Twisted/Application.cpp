#include "pch.h"
#include "Application.h"

#include "Debug/Logger.h"
#include "Twisted/GameCore.h"
#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Game/Systems/RenderSystem.h"
#include "Editor/EditorCore.h"
#include "Collections/Color.h"

namespace Twisted
{
	Application::Application()
	{
	}
	Application::~Application()
	{
		Render_OpenGL::Terminate();
	}

	void Application::Start(RuntimeBase* runtime)
	{
		if (IsRunning())
		{
			TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}
		Runtime = runtime;

		Init();
		Run();
	}

	void Application::Stop()
	{
		m_isRunning = false;
	}

	void Application::Init()
	{
		Time.Init();

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

		Game.Ecs.AddSystem<Twisted::RenderSystem>();
		Windows.CreateNewWindow(Runtime->Params.window1Title, Runtime->Params.monitorWidth, Runtime->Params.monitorHeight);
		Resources.LoadResources(Runtime->Params.rootFolder);

		Editor.Init(this);
		Runtime->OnInit(this);
	}

	void Application::Run()
	{
		m_isRunning = true;

		Runtime->OnBeforeRun(this);
		while (IsRunning())
		{
			Time.UpdateClocks();

			if (Time.IsNextFrame())
			{
				Time.IncreaseFrameCount();
				FrameUpdate();
				Time.ResetFrameTime();
			}
		}
		Terminate();
	}

	void Application::FrameUpdate()
	{
		Editor.Update(this);

		Runtime->OnRun(this);

		glfwMakeContextCurrent(Windows.GetWindow()->Pointer);
		Render_OpenGL::ClearWindow(Colors::blue);

		Game.Ecs.UpdateSystems(this);
		Editor.Render(this);

		Windows.UpdateWindow();
	}

	void Application::Terminate()
	{
		Editor.Terminate(this);
	}
}
#include "twistedpch.h"
#include "AppBase.h"
#include "Twisted/Rendering/OpenGL/Render_OpenGL.h"
#include "Twisted/Game/Systems/RenderSystem.h"

namespace Twisted
{
	AppBase::AppBase()
	{
		m_time.Init();
	}

	AppBase::~AppBase()
	{
		Terminate();
	}

	void AppBase::Start(const AppParams& params, RuntimeBase* runtime)
	{
		if (IsRunning())
		{
			TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}

		m_runtime = runtime;
		if (!Init(params))
			return;

		Run();
	}

	void AppBase::Run()
	{
		if (m_runtime)
			m_runtime->OnBeforeRun(this);

		while (IsRunning())
		{
			WindowAPI::PollEvents();
			RenderAPI::ClearWindow(Colors::blue);

			m_world->UpdateFrame(this,m_time);

			WindowAPI::SwapBuffers(GetWindow());

			if (m_runtime)
				m_runtime->OnRun(this);
		}
	}

	void AppBase::CreateNewWindow(const AppParams& params)
	{
		m_window = WindowAPI::CreateNewWindow(params.windowParams, this);
		glfwMakeContextCurrent(GetWindow()->Pointer);
	}
	void AppBase::CreateWorld()
	{
		m_world = std::make_shared<World>();
		m_world->AddSystem<Twisted::RenderSystem>(); //TODO: somewhere else
	}
	void AppBase::LoadResources(const AppParams& params)
	{
		//m_resources.LoadResources(params.rootFolder);
	}

	bool AppBase::Init(const AppParams& params)
	{
		if (
			Logger::Init() &&
			RenderAPI::InitGLFW()
			)
		{
			CreateNewWindow(params);
			CreateWorld();
			LoadResources(params);

			if (m_runtime)
				m_runtime->OnInit(this);

			m_isRunning = true;

			TWISTED_INFO("Application Init Success!");
			return true;
		}
		else
		{
			TWISTED_ERROR("Application Init failure");
			return false;
		}


	}

	void AppBase::Terminate()
	{
		RenderAPI::Terminate();
	}
}
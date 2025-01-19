#include "StandaloneApp.h"
#include "AppCore.h"
#include "Rendering/RenderingAPI.h"
#include "Windows/Window.h"

#include "Utils/FileUtils.h"
#include <GLFW/glfw3.h>

namespace Twisted::Standalone
{
	void StandaloneApp::Run()
	{
		if (m_isRunning)
		{
			TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}

		if (!Init())
			return;

		m_monitor = Monitor::CreateMonitorInfo();
		m_window = Window::CreateNewWindow("Test Standalone App", nullptr, Vec2i(800, 800), Vec2i(400, 400));

		glfwMakeContextCurrent(GetWindow()->GetPointer());
		m_window->CloseWindowEvent.AddListener([&]()
			{
				this->Stop();
			});

		LoadResources();

		m_isRunning = true;
		while (m_isRunning)
		{
			m_window->PollEvents();
			m_window->ClearWindow(Colors::blue);

			if (m_world)
				m_world->UpdateFrame(this);

			m_window->SwapBuffers();
		}
	}

	bool StandaloneApp::Init()
	{
		if (Logger::Init() && RenderAPI::InitGLFW())
		{
			m_time.Start();
			TWISTED_INFO("Application Init Success!");
			return true;
		}

		TWISTED_ERROR("Application Init failure");
		return false;
	}

	void StandaloneApp::LoadResources()
	{
		//load assets


		//load world
		m_world == std::make_shared<World>();
	}

}
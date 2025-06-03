#include "twistedpch.h"
#include "Application.h"
#include "Twisted.h"

#include "Window.h"
#include "Monitor.h"
#include "Layer.h"

namespace Twisted
{
	void Application::Run()
	{
		if (m_isRunning)
		{
			Twisted::TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}

		if (!Twisted::Init())
			return;

		for (auto& layer : m_layers)
			layer->OnInit();

		m_time.Start();

		m_monitor = Monitor::CreateMonitorInfo();
		m_window = Window::CreateNewWindow("Temp name", nullptr, Vec2i(800, 600), Vec2i(400, 400));

		m_window->CloseWindowEvent.AddListener([&]() {
			this->Stop();
			});

		for (auto& layer : m_layers)
			layer->OnBeforeRun();

		m_isRunning = true;
		while (m_isRunning)
		{
			m_window->PollEvents();
			//m_window->ClearWindow();

			for (auto& layer : m_layers)
				layer->OnFrameBegin();

			for (auto& layer : m_layers)
				layer->OnFrame();

			for (auto& layer : m_layers)
				layer->OnFrameEnd();

			m_window->SwapBuffers();
		}

		for (auto& layer : m_layers)
			layer->OnTerminate();

		Twisted::Terminate();
	}
}
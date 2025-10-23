#include "Application.h"
#include "Twisted.h"
#include "Debug/Logger.h"

namespace Twisted
{
	void Application::Run()
	{
		LOGGER_INIT();

		if (m_isRunning)
		{
			TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}

		OnCreate();

		for (auto& layer : m_layers)
			layer->Init();

		m_time.Start();

		OnBeforeRun();

		m_isRunning = true;
		while (m_isRunning)
		{
			OnFrame();
		}
		
		OnTerminate();
	}
}
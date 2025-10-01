#include "Application.h"
#include "Twisted.h"
#include "Logger.h"

namespace Twisted
{
	void Application::Run()
	{
		Logger::Init(); //makro for these debug

		if (m_isRunning)
		{
			TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}

		if (m_runtime)
			m_runtime->OnCreate();

		for (auto& layer : m_layers)
			layer->Init();

		m_time.Start();

		if (m_runtime)
			m_runtime->OnBeforeRun();

		m_isRunning = true;
		while (m_isRunning)
		{
			if (m_runtime)
				m_runtime->OnFrame();
		}

		if (m_runtime)
			m_runtime->OnTerminate();
	}
}
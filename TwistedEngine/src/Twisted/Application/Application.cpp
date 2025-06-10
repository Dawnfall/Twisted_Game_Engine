#include "Application.h"
#include "Twisted.h"


namespace Twisted
{
	void Application::Run()
	{
		if (m_isRunning)
		{
			Twisted::TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}
		if (m_runtime)
			m_runtime->OnCreate();

		Logger::Init(); //makro for these debug
		m_time.Start();

		if (m_runtime)
			m_runtime->OnInit();

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
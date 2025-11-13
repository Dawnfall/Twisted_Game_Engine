#include "Application.h"
#include "Twisted.h"
#include "Debug/Logger.h"

namespace Twisted
{
	void Application::Run()
	{
		if (m_isRunning)
		{
			TWISTED_WARN("Cannot start application; Application already running!");
			return;
		}

		LOGGER_INIT();

		OnInit();
		OnBeforeRun();

		m_isRunning = true;
		while (m_isRunning)
		{
			OnBeginFrame();
			OnFrame();
			OnEndFrame();
		}

		OnTerminate();
	}
}


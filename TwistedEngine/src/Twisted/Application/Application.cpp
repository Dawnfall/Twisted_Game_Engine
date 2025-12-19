#include "Application.h"
#include "Twisted.h"
#include "Debug/Logger.h"
#include "Twisted/TObject.h"
#include "TimeService.h"

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

		m_timeService = AddService<TimeService>();

		for (auto& processor : m_processors)
			processor->OnInit();

		for (auto& processor : m_processors)
			processor->OnBeforeRun();

		m_isRunning = true;
		while (m_isRunning)
		{
			m_timeService->Update();

			for (auto& processor : m_processors)
				processor->OnFrameBegin();
			for (auto& processor : m_processors)
				processor->OnFrame();
			for (auto& processor : m_processors)
				processor->OnFrameEnd();
		}

		for (auto& processor : m_processors)
			processor->OnTerminate();

		m_processors.clear();
		m_services.clear();

		TObject::DestroyAll(); //TODO... can be moved to some GC
	}
}


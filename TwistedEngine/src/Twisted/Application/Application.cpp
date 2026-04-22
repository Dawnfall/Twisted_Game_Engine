#include "Twisted/Application/Application.h"
#include "Twisted.h"
#include "Debug/Logger.h"
#include "Twisted/TObject.h"
#include "Twisted/Application/TimeService.h"

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


		for (auto& service : m_services)
			service->OnInit();

		for (auto& service : m_services)
			service->OnBeforeRun();

		m_isRunning = true;
		while (m_isRunning)
		{
			for (auto& service : m_services)
				service->OnFrameBegin();
			for (auto& service : m_services)
				service->OnFrame();
			for (auto& service : m_services)
				service->OnFrameEnd();
		}

		for (auto& service : m_services)
			service->OnTerminate();

		m_services.clear();

		TObject::DestroyAll(); //TODO... can be moved to some GC
	}
}


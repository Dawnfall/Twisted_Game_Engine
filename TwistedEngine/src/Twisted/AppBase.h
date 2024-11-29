#pragma once
#include "twistedpch.h"
#include "Twisted/RuntimeBase.h"
#include "Twisted/Managers/ResourceManager.h"
#include "Twisted/Windows/Monitor.h"
#include "Twisted/Windows/Window.h"
#include "Twisted/Game/World.h"
#include "Twisted/Managers/TimeManager.h"

#include "Utils/Event.h"

#include "AppCore.h"

namespace Twisted
{
	class TWISTED_API AppBase
	{
	public:
		virtual void Run(RuntimeBase* runtime) = 0;
		void Stop() { m_isRunning = false; }
		bool IsRunning()const { return m_isRunning; }

		ResourceManager& GetResources() { return m_resources; }
		RuntimeBase* GetRutime() { return m_runtime; }
		TimeManager& GetTime() { return m_time; }
		SRef<Window> GetWindow() { return m_window; }
		SRef<World> GetActiveWorld() { return m_activeWorld; }
		SRef<Monitor> GetMonitor() { return m_monitor; }

	protected:
		SRef<Monitor> m_monitor = nullptr;
		SRef<Window> m_window = nullptr;
		SRef<World> m_activeWorld = nullptr;

		RuntimeBase* m_runtime = nullptr;
		ResourceManager m_resources;
		TimeManager m_time;

		bool m_isRunning = false;
	};
}
#pragma once
#include "Twisted/Managers/ResourceManager.h"
#include "Windows/Monitor.h"
#include "Windows/Window.h"
#include "Twisted/Game/World.h"
#include "Twisted/Managers/TimeManager.h"
#include "Rendering/FrameBuffer.h"

#include "Utils/Event.h"

#include "AppCore.h"

namespace Twisted
{
	class TWISTED_API AppBase
	{
	public:
		virtual void Run() = 0;
		void Stop() { m_isRunning = false; }

		ResourceManager& GetResources() { return m_resources; }
		TimeManager& GetTime() { return m_time; }
		SRef<Window> GetWindow() { return m_window; }
		SRef<Monitor> GetMonitor() { return m_monitor; }
		FrameBuffer* GetFrameBuffer() { return m_frameBuffer.get(); }

	protected:

		SRef<Twisted::FrameBuffer> m_frameBuffer = nullptr;
		SRef<Monitor> m_monitor = nullptr;
		SRef<Window> m_window = nullptr;
		ResourceManager m_resources;
		TimeManager m_time;

		bool m_isRunning = false;
	};
}
#pragma once
#include "AppCore.h"
#include "Layer.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Monitor.h"
#include "Window.h"

namespace Twisted
{
	class AppData;

	class TWISTED_API Application
	{
	public:
		void Run();
		void Stop() { m_isRunning = false; }

		InputManager& GetInput() { return m_input; }
		TimeManager& GetTime() { return m_time; }
		Monitor* GetMonitor() { return (m_monitor) ? m_monitor.get() : nullptr; }
		Window* GetWindow() { return (m_window) ? m_window.get() : nullptr; }

		template<typename T>
		T* AddLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Layer must inherit from Layer class");
			m_layers.emplace_back(std::make_unique<T>(this));
			return static_cast<T*>(m_layers.back().get());
		}

	private:
		URef<Window> m_window;
		URef<Monitor> m_monitor;
		InputManager m_input;
		TimeManager m_time;

		std::vector<URef<Layer>> m_layers;
		bool m_isRunning = false;
	};

	//	void StandaloneApp::LoadResources()
//	{
//		//m_world = std::make_shared<World>();
//		//m_world->AddSystem<SCameraController>(app);
//		//auto window = app->GetWindow();
//		//Vec3f cameraPos(0.0, 0.0f, 0.0f);
//		//Vec3f cubePos(0.0f, 0.0f, -2.0f);
//		//load assets
//
//
//		//load world
//	}
}
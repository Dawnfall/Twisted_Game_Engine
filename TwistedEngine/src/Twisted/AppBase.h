#pragma once
#include "twistedpch.h"
#include "AppParams.h"
#include "Twisted/RuntimeBase.h"
#include "Twisted/Managers/InputManager.h"
#include "Twisted/Managers/ResourceManager.h"
#include "Twisted/Windows/Window.h"
#include "Twisted/Game/World.h"
#include "Twisted/Managers/TimeManager.h"

#include "Utils/Event.h"

#include "Windows/WindowAPI.h"

#include "AppCore.h"

namespace Twisted
{
	class TWISTED_API AppBase
	{
	public:
		AppBase();
		~AppBase();

		virtual void Start(const AppParams& params,RuntimeBase* runtime);
		void Stop() { m_isRunning = false; }

		bool IsRunning()const { return m_isRunning; }

		InputManager& GetInput() { return m_input; }
		ResourceManager& GetResources() { return m_resources; }
		RuntimeBase* GetRutime() { return m_runtime; }
		TimeManager& GetTime() { return m_time; }
		std::shared_ptr<Window> GetWindow() { return m_window; }
		std::shared_ptr<World> GetWorld() { return m_world; }

		Event<> CloseWindowEvent;
		Event<> WindowResizeEvent;
		Event<int> KeyCallbackEvent;
		Event<int> ButtonCallbackEvent;

	protected:

		virtual bool Init(const AppParams& params);
		virtual void Terminate();
		virtual void CreateNewWindow(const AppParams& params);
		virtual void CreateWorld();
		virtual void LoadResources(const AppParams& params);
		virtual void Run();

	protected:

		RuntimeBase* m_runtime = nullptr;
		ResourceManager m_resources;
		TimeManager m_time;
		InputManager m_input;
		std::shared_ptr<World> m_world;
		std::shared_ptr<Window> m_window;

		bool m_isRunning = false;
	};
}
#pragma once

#include <vector>

#include "Dawn/GameCore.h"
#include "Dawn/RenderCore.h"
#include "Dawn/Managers/WindowManager.h"
#include "SystemsCore.h"
#include "Dawn/Managers/ResourceManager.h"

namespace Dawn
{
	class Application;

	class RuntimeBase
	{
	public:
		virtual void OnInit(Application& app) = 0;
		virtual void OnBeforeRun(Application& app) = 0;
		virtual void OnRun(Application& app) = 0;
	};

	class Application
	{
	public:
		Application();
		~Application();

		void Start(RuntimeBase& runtime);
		void Stop() { m_isRunning = false; }

		bool IsValid() { return m_isValid; }
		bool IsRunning() { return m_isRunning; }

		WindowManager WindowManager;
		ResourceManager ResourceManager;

		GameCore Game;
		RenderCore Renderer;

	private:
		RuntimeBase* m_runtime = nullptr;
		bool m_isValid = false;
		bool m_isRunning = false;

		void Init();
		void BeforeStart();
		void Run();
	};
}
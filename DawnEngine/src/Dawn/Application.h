#pragma once

#include "Dawn/Game/GameCore.h"
#include "WindowManager.h"
#include "../StartParams.h"

namespace Dawn
{
	class Application
	{
	public:
		Application();
		~Application();

		void Start(const StartParams& startParams);
		void Stop() { m_isRunning = false; }

		bool IsValid() { return m_isValid; }
		bool IsRunning() { return m_isRunning; }

		WindowManager WindowManager;
	private:
		bool m_isValid = false;
		bool m_isRunning = false;


		void Init();
		void Run();

		//debug...
		void OnStart();
		void OnRun();
	};
}
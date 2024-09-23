#pragma once

#include "Twisted/GameCore.h"
#include "Twisted/Managers/WindowManager.h"
#include "SystemsCore.h"
#include "Twisted/Managers/ResourceManager.h"
#include "Twisted/Managers/TimeManager.h"
#include "Twisted/AppParams.h"
#include "Twisted/RuntimeBase.h"
#include "Editor/EditorCore.h"
#include "Editor/Editor.h"

namespace Twisted
{
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
		TimeManager TimeManager;

		GameCore Game;

	private:
		RuntimeBase* m_runtime = nullptr;
		bool m_isValid = false;
		bool m_isRunning = false;

		void Run();
		void Init();
		void FrameUpdate();
		void Terminate();
	};
}
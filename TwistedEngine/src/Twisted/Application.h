#pragma once

#include "pch.h"
#include "Twisted/RuntimeBase.h"

#include "Twisted/Managers/WindowManager.h"
#include "Twisted/Managers/ResourceManager.h"
#include "Twisted/Managers/TimeManager.h"
#include "Twisted/GameCore.h"
#include "Editor/Editor.h"

namespace Twisted
{
	class Application
	{
	public:
		Application();
		~Application();

		void Start(RuntimeBase* runtime);
		void Stop();

		bool IsValid() { return m_isValid; }
		bool IsRunning() { return m_isRunning; }

		TimeManager Time;
		ResourceManager Resources;
		WindowManager Windows;
		GameCore Game;

		RuntimeBase* Runtime = nullptr;
		Editor::Editor Editor;

	private:
		bool m_isValid = false;
		bool m_isRunning = false;

		void Run();
		void Init();
		void FrameUpdate();
		void Terminate();
	};
}



#pragma once

#include "Twisted/Window.h"
#include "Debug/Logger.h"
#include "Collections/Color.h"
#include "InputManager.h"

namespace Twisted
{
	class WindowManager
	{
	public:
		bool CreateNewWindow(const std::string& title, unsigned int width, unsigned int height);
		void UpdateWindow();
		bool CloseWindow();

		std::shared_ptr<Window> GetWindow()
		{
			return m_window;
		}

		InputManager m_inputManager;
		Event<> closeWindowEvent;
	private:
		std::shared_ptr<Window> m_window;

		void SetCallbacks();
		void ProcessEvents();
	};
}

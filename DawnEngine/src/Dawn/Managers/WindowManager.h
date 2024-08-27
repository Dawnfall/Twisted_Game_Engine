#pragma once

#include <memory>
#include <vector>

#include "Dawn/Window.h"
#include "Debug/Logger.h"
#include "Collections/Color.h"
#include "InputManager.h"

namespace Dawn
{
	class WindowManager
	{
	public:
		bool CreateNewWindow(const std::string& title, unsigned int width, unsigned int height);
		const std::vector<std::shared_ptr<Window>>& GetWindows() { return m_windows; }
		std::shared_ptr<Window> GetWindow(int id);
		bool CloseWindow(unsigned int index);

		void UpdateWindows();

		InputManager m_inputManager;
		Event<> closeWindowEvent;
	private:
		std::vector<std::shared_ptr<Window>> m_windows;
		unsigned m_nextID = 1;

		std::shared_ptr<Window> GetWindow(GLFWwindow* pointer);
		void SetCallbacks();
		void ProcessEvents();
	};
}

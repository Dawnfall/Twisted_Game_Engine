#pragma once

#include <memory>
#include <vector>

#include "Dawn/Window.h"
#include "Debug/Logger.h"
#include "Collections/Color.h"

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

		Event<> closeWindowEvent;
	private:
		std::shared_ptr<Window> GetWindow(GLFWwindow* pointer);
		void SetCallbacks();
		void ProcessEvents();

		//TODO: make these as lamba in setcallbacks()
		static bool IsKeyPressed(GLFWwindow* window, int key);
		static bool IsKeyReleased(GLFWwindow* window, int key);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		std::vector<std::shared_ptr<Window>> m_windows;
		unsigned m_nextID = 1;
	};
}

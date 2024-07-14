#pragma once

#include <memory>
#include <vector>

#include "Dawn/Window.h"
#include "Debug/Logger.h"

namespace Dawn
{
	class WindowManager
	{
	public:
		bool CreateNewWindow(const WindowData& windowData);
		bool DestroyWindow();

		void UpdateWindow();

		void SetVsync(int deltaFrames);
		void ClearWindow(Color color);
		void CloseWindow();
		bool IsWindowClosing();

		Event<> closeWindowEvent;
	private:
		void ProcessEvents();

		std::unique_ptr<Window> m_window;

		static bool IsKeyPressed(GLFWwindow* window, int key);
		static bool IsKeyReleased(GLFWwindow* window, int key);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		void SetCallbacks();

	};
}

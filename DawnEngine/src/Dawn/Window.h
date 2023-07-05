#pragma once

#include <GLFW/glfw3.h>
#include <string>


namespace Dawn
{
	struct WindowData
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowData(const std::string& title = "Dawn", unsigned int width = 1280, unsigned int height = 720) :
			Title(title),
			Width(width),
			Height(height)
		{}
	};

	class Window
	{
	public:
		Window(const WindowData& windowData = WindowData());
		~Window();

		void OnUpdate();
		void SetVsync(int deltaFrames) { glfwSwapInterval(deltaFrames); }
		
		static Window* Create(const WindowData& windowData = WindowData());

		const WindowData& GetWindowData() { return m_windowData; }

		static bool Init();
		static void Terminate();

		bool ShouldClose()
		{
			return glfwWindowShouldClose(m_window);
		}

		//***************
		// Callbacks

		static bool IsKeyPressed(GLFWwindow* window, int key);
		static bool IsKeyReleased(GLFWwindow* window, int key);
		static void ErrorCallback(int code, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void WindowCloseCallback(GLFWwindow* window);
		static void WindowResizeCallback(GLFWwindow* window, int width, int height);

	private:
		GLFWwindow* m_window = nullptr;
		WindowData m_windowData;

		void ProcessEvents();
	};
}

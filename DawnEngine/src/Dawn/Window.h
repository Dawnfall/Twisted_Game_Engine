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
	private:
		GLFWwindow* m_window = nullptr;
		WindowData m_windowData;

	public:
		Window(const WindowData& windowData = WindowData());
		~Window();

		void OnUpdate();
		
		static Window* Create(const WindowData& windowData = WindowData());

		const WindowData& GetWindowData() { return m_windowData; }
	};
}

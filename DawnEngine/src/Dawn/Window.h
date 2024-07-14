#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include "Dawn/Rendering/RenderContext.h"
#include "Event.h"
namespace Dawn
{
	class RenderContext;

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
		Window(const WindowData& data,GLFWwindow* window);
		~Window();
		
		WindowData Data;
		GLFWwindow* Pointer = nullptr;

		Event<> closeWindowEvent;
	};
}

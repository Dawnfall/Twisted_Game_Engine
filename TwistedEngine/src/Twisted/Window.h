#pragma once

#include "pch.h"
#include <GLFW/glfw3.h>
#include "Utils/Event.h"

namespace Twisted
{
	struct Window
	{
	public:
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		GLFWwindow* Pointer = nullptr;

		Window(GLFWwindow* window, const std::string& title, unsigned int width, unsigned int height) :
			Title(title),
			Width(width),
			Height(height),
			Pointer(window)
		{}

		Event<> closeWindowEvent;
	};
}

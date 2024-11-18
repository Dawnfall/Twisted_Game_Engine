#pragma once

#include "twistedpch.h"
#include "Utils/Event.h"
#include "AppCore.h"

namespace Twisted
{
	struct TWISTED_API Window
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
	};
}

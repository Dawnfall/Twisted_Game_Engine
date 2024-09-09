#pragma once

#include "pch.h"
#include <GLFW/glfw3.h>
#include "Utils/Event.h"

namespace Twisted
{
	struct Window
	{
	public:
		unsigned int ID;
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		GLFWwindow* Pointer = nullptr;

		Window(GLFWwindow* window, unsigned int Id, const std::string& title, unsigned int width, unsigned int height) :
			Title(title),
			Width(width),
			Height(height),
			Pointer(window),
			ID(Id)
		{}

		Event<> closeWindowEvent;
	};
}

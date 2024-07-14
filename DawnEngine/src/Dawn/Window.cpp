#include "Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Dawn/Core_macros.h"
#include "Debug/Logger.h"

//TODO:... error handling

namespace Dawn
{

	Window::Window(const WindowData& windowData, GLFWwindow* window) :
		Data(windowData),
		Pointer(window)
	{}

	Window::~Window()
	{}
}
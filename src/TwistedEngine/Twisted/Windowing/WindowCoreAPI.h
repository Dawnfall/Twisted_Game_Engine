#pragma once

#include "AppCore.h"
//#ifdef GLFW_INCLUDE_NONE

#include "Twisted/Windowing/WindowsService.h"
#include <GLFW/glfw3.h>
#include "Debug/Logger.h"

namespace Twisted::Windowing
{
	inline void Init()
	{
		if (!glfwInit())
		{
			TWISTED_ERROR("GLFW init failure; RenderCore Init failure!");
			return;
		}
		glfwSetErrorCallback([](int code, const char* description) {
			TWISTED_ERROR(description);
			});

		TWISTED_INFO("GLFW init success");
	}

	inline void Terminate()
	{
		glfwTerminate();
	}

	inline void PollEvents(Window& window)
	{
		glfwPollEvents();

		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetRawPointer()), &x, &y);
		Input::GetInstance().UpdateMousePosition(float(x), window.GetSize().y - float(y));

	}

}
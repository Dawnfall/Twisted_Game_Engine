#include "WindowLayer.h"
#include "Logger.h"

#include <GLFW/glfw3.h>

namespace Twisted
{
	bool WindowLayer::InitGLFW()
	{
		if (!glfwInit())
		{
			TWISTED_ERROR("GLFW init failure; RenderCore Init failure!");
			return false;
		}
		glfwSetErrorCallback([](int code, const char* description) {
			TWISTED_ERROR(description);
			});

		TWISTED_INFO("GLFW init success");
		TWISTED_INFO("RenderCore Init success!");

		return true;
	}
	void WindowLayer::TerminateGLFW()
	{
		glfwTerminate();
	}
}
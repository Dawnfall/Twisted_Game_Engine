#include "AppCore.h"
#ifdef GLFW_INCLUDE_NONE

#include "Twisted/Windowing/WindowLayer.h"
#include <GLFW/glfw3.h>
#include "Debug/Logger.h"

namespace Twisted
{
	WindowLayer::WindowLayer(Application* app) :Layer(app)
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
		TWISTED_INFO("RenderCore Init success!");
	}
	WindowLayer::~WindowLayer() 
	{ 
		glfwTerminate();
	}
}

#endif

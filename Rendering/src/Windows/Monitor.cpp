#include "renderingpch.h"
#include "Monitor.h"

#include <GLFW/glfw3.h>

namespace Twisted
{
	Monitor::Monitor(GLFWmonitor* pointer)
	{
		Pointer = pointer;

		Name = glfwGetMonitorName(pointer);
		glfwGetMonitorPos(pointer, &Position.x, &Position.y);
		glfwGetMonitorPhysicalSize(pointer, &Size.x, &Size.y);

		const GLFWvidmode* mode = glfwGetVideoMode(pointer);
		Resolution = Vec2i(mode->width, mode->height);
		RefreshRate = mode->refreshRate;
	}

	SRef<Monitor> Monitor::CreateMonitorInfo()
	{
		GLFWmonitor* primaryMonitorPointer = glfwGetPrimaryMonitor();
		if (primaryMonitorPointer)
			return std::make_shared<Monitor>(primaryMonitorPointer);
		return nullptr;
	}
}
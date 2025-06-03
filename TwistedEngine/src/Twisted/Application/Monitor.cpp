#include "twistedpch.h"
#include "Monitor.h"

namespace Twisted
{
	Monitor::Monitor(GLFWmonitor* pointer) :
		Pointer(pointer)
	{
		Name = glfwGetMonitorName(pointer);
		glfwGetMonitorPos(pointer, &Position.x, &Position.y);
		glfwGetMonitorPhysicalSize(pointer, &Size.x, &Size.y);

		const GLFWvidmode* mode = glfwGetVideoMode(pointer);
		Resolution = Vec2i(mode->width, mode->height);
		RefreshRate = mode->refreshRate;
	}

	URef<Monitor> Monitor::CreateMonitorInfo()
	{
		GLFWmonitor* primaryMonitorPointer = glfwGetPrimaryMonitor();
		if (primaryMonitorPointer)
		{
			return std::make_unique<Monitor>(primaryMonitorPointer);
		}
		return nullptr;
	}
}
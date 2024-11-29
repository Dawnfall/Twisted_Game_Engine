#pragma once
#include "twistedpch.h"
#include "Collections/Geometry.h"
namespace Twisted
{
	struct Monitor
	{
		Monitor(GLFWmonitor* pointer)
		{
			Pointer = pointer;

			Name = glfwGetMonitorName(pointer);
			glfwGetMonitorPos(pointer, &Position.x, &Position.y);
			glfwGetMonitorPhysicalSize(pointer, &Size.x, &Size.y);

			const GLFWvidmode* mode = glfwGetVideoMode(pointer);
			Resolution = Vec2i(mode->width, mode->height);
			RefreshRate = mode->refreshRate;
		}

		GLFWmonitor* Pointer;
		std::string Name;
		Vec2i Position;
		Vec2i Size;
		Vec2i Resolution;
		int RefreshRate;

		static SRef<Monitor> CreateMonitorInfo()
		{
			GLFWmonitor* primaryMonitorPointer = glfwGetPrimaryMonitor();
			if (primaryMonitorPointer)
				return std::make_shared<Monitor>(primaryMonitorPointer);
			return nullptr;
		}
	};
}

#pragma once
#include <Utils/GlmUtils.h>
#include <AppCore.h>
#include <string>

class GLFWmonitor;

namespace Twisted
{
	struct Monitor
	{
		Monitor(GLFWmonitor* pointer);

		GLFWmonitor* Pointer;
		std::string Name;
		Vec2i Position;
		Vec2i Size;
		Vec2i Resolution;
		int RefreshRate;

		static SRef<Monitor> CreateMonitorInfo();
	};
}

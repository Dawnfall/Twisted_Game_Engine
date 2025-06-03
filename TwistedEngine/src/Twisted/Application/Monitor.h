#pragma once
#include "AppCore.h"
#include "Utils/Event.h"
#include "Utils/GlmUtils.h"

class GLFWmonitor;
namespace Twisted
{

	class TWISTED_API Monitor
	{
	public:
		Monitor(GLFWmonitor* pointer);

		GLFWmonitor* Pointer;
		std::string Name = "";
		Vec2i Position{ 0,0 };
		Vec2i Size{ 0,0 };
		Vec2i Resolution{ 0,0 };
		int RefreshRate{ -1 };

		static URef<Monitor> CreateMonitorInfo();
	};
}

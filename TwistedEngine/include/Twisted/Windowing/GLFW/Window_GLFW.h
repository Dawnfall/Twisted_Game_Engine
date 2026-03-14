#ifdef TWISTED_GLFW
#pragma once
#include <GLFW/glfw3.h>

namespace Twisted
{
	struct WindowBackend
	{
		std::unique_ptr<GLFWWindow> m_data;
	};



}

#endif
#pragma once

#include "twistedpch.h"

namespace Twisted
{
	struct WindowParams
	{
		std::string windowTitle = "Dawn1";
		unsigned int monitorWidth = 1280;
		unsigned int monitorHeight = 720;
	};

	struct AppParams
	{
		std::string rootFolder = "";
		WindowParams windowParams;
	};
}
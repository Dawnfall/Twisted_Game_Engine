#pragma once

#include "twistedpch.h"
#include "AppCore.h"

namespace Twisted
{
	struct TWISTED_API WindowParams
	{
		std::string windowTitle = "Dawn1";
		unsigned int monitorWidth = 1280;
		unsigned int monitorHeight = 720;
	};

	struct TWISTED_API AppParams
	{
		std::string rootFolder = "";
		WindowParams windowParams;
	};
}
#pragma once

#include <memory>
#include <vector>

#include "Dawn/Window.h"
#include "Debug/Logger.h"

namespace Dawn
{
	class RenderCore
	{
	public:
		static bool Init();
		static void Terminate();

	private:
		static void ErrorCallback(int code, const char* description);
	};
}

#pragma once

#include "AppCore.h"

#include "Windows.h"
#include "Debug/Logger.h"

#include <glad/glad.h>       

namespace Twisted
{
	struct WindowBackend
	{
		HWND hwnd = nullptr;
		HDC hdc = nullptr;
		HGLRC glrc = nullptr;
	};

	HGLRC createContext(HDC hdc);
	bool setPixelFormat(HDC windowHandle);
}
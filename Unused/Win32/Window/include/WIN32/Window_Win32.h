#pragma once

#include "AppCore.h"

#include "Windows.h"
#include "Debug/Logger.h"

#ifdef TWISTED_BACKEND_OPENGL
#include <glad/glad.h>
#endif

namespace Twisted
{
	struct WindowBackend
	{
		HWND hwnd = nullptr;
		HDC hdc = nullptr;
#ifdef TWISTED_BACKEND_OPENGL
		HGLRC glrc = nullptr;
#endif

		WindowBackend(const std::string& title, Vec2i& size, Vec2i position);
		~WindowBackend();
	};

	HWND createWindowHandle(const std::string& title, Vec2i size, Vec2i position);

#ifdef TWISTED_BACKEND_OPENGL
	HGLRC createContext(HDC hdc);
	bool setPixelFormat(HDC windowHandle);
#endif
}

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

		WindowBackend(const std::string& title, Vec2i& size, Vec2i position);
		~WindowBackend();
	};

	HGLRC createContext(HDC hdc);
	bool setPixelFormat(HDC windowHandle);
	HWND createWindowHandle(const std::string& title, Vec2i size, Vec2i position);
}
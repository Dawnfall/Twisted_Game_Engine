#pragma once

#include "Utils/GlmUtils.h"
#include "Twisted/Data/Color.h"
#include <string>
#include <filesystem>

namespace Twisted::Editor::Constants
{
	// Project Selection

	const std::string LOADUP_WIN_TITLE = "Project Selection";
	const Vec2i LOADUP_WIN_SIZE = Vec2i(1000, 800);
	const Vec2i LOADUP_WIN_POS = Vec2i(400, 400);

	// Window

	const std::string EDITOR_WINDOW_TITLE = "Twisted Editor";
	const Vec2i WINDOW_DEFAULT_SIZE{ 1280, 720 };
	const Vec2i WINDOW_DEFAULT_POS{ 100, 100 };

	const std::filesystem::path ASSETS_SUBFOLDER = "Assets";
	const Twisted::Color WINDOW_CLEAR_COLOR{ 0.9f, 0.3f, 0.2f, 1.0f };

}
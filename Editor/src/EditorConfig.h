#pragma once
#include "Utils/GlmUtils.h"
#include "Serialization/XmlSerializer.h"

#include <string>
#include <filesystem>

namespace Twisted::Editor
{
	const std::filesystem::path editorConfigFile = "editor.config";

	const std::string WIN_TITLE_NAME = "window_title";
	const std::string WIN_SIZE_NAME = "window_size";
	const std::string WIN_POS_NAME = "window_pos";
	const std::string IMGUI_SIZE = "imgui_size";
	const std::string IMGUI_DATA = "imgui_data";
	const std::string FRAMEBUFFER_SIZE = "framebuffer_size";

	struct EditorConfig
	{
		std::string windowTitle = "Twisted Editor";
		Vec2i windowSize{ 1280, 720 };
		Vec2i windowPos{ 100, 100 };
		Vec2i framebufferSize{ 1280, 720 };

		unsigned int imguiSize = 0;
		char* imguiData = nullptr;
	};
}
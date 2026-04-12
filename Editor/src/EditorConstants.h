#pragma once

#include "Utils/GlmUtils.h"
#include "Data/Color.h"
#include <string>
#include <filesystem>
#include <imgui.h>

namespace Twisted::Editor::Constants
{
	// Project Selection
	inline const std::filesystem::path PROJECT_ROOT = PROJECT_ROOT_PATH;
	inline const std::filesystem::path CONFIG_FILE_PATH = PROJECT_ROOT / "Data/EditorConfig/editor.config";
	inline const std::string WIN_SIZE_NAME = "window_size";
	inline const std::string WIN_POS_NAME = "window_pos";
	inline const std::string WIN_MAXIMIZED_NAME = "window_maximized";
	inline const std::string RECENT_PROJECTS_NAME = "recent_projects";

	// Window

	inline const std::string EDITOR_WINDOW_TITLE = "Twisted Editor";
	inline const Vec2i WINDOW_DEFAULT_SIZE{ 1280, 720 };
	inline const Vec2i WINDOW_DEFAULT_POS{ 100, 100 };

	inline const std::filesystem::path ASSETS_SUBFOLDER = "Assets";
	inline const Twisted::Color WINDOW_CLEAR_COLOR{ 0.9f, 0.3f, 0.2f, 1.0f };

	//Drag

	inline const std::string ENTITY_DRAG_TYPE = "Entity_drag";
	inline const std::string ASSET_DRAG_TYPE = "Asset_drag";
	inline const std::string OBJECT_DRAG_TYPE = "Object_drag";

	inline const std::string mainDockSpaceLabel = "MainDockSpace";
	inline ImGuiWindowFlags panelFlags =
		ImGuiWindowFlags_NoCollapse;     // optional: prevent collapsing
	//| ImGuiWindowFlags_NoDocking;   // prevents undocking

	inline ImGuiWindowFlags dockFlags =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavFocus;


}

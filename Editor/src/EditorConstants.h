#pragma once

#include "Utils/GlmUtils.h"
#include "Data/Color.h"
#include <string>
#include <filesystem>
#include "imgui.h"

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

	//Drag

	const std::string ENTITY_DRAG_TYPE = "Entity_drag";
	const std::string ASSET_DRAG_TYPE = "Asset_drag";
	const std::string OBJECT_DRAG_TYPE = "Object_drag";

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
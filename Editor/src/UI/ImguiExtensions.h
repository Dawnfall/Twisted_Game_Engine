#pragma once
#define NOMINMAX
#include <algorithm>
#include <filesystem>

//#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_internal.h"

namespace Im
{
	const float PI = 3.141592f;
	const int DEFAULT_ICON_SIZE = 32;

	constexpr float GUI_ELEMENT_SIZE()
	{
		return std::max<float>(GImGui->FontSize + 10.0f, 24.0f);
	}
	float ICON_SIZE();

	//void Init(GLFWwindow* windowPointer);
	void Init(void* windowPointer);

	void SetFlags();
	void SetStyle();
	void Terminate();
	void Render();

	void StartFrame();
	void EndFrame();

	//from filebrowser
	bool PathBox(const char* label, std::filesystem::path& path, char* pathBuffer, ImVec2 size_arg);
	bool FavoriteButton(const char* label, bool isFavorite);
	bool FolderNode(const char* label, ImTextureID icon, bool& clicked);
	bool FileIcon(const char* label, bool isSelected, ImTextureID icon, ImVec2 size);

	// to use string
	bool InputText(const std::string& label, std::string& value, ImGuiInputTextFlags flags = 0);
}
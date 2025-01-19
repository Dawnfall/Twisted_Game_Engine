#pragma once

namespace Im
{
	const float PI = 3.141592f;
	const int DEFAULT_ICON_SIZE = 32;

	constexpr float GUI_ELEMENT_SIZE()
	{
		return std::max(GImGui->FontSize + 10.0f, 24.0f);
	}
	float ICON_SIZE();

	void Init(GLFWwindow* windowPointer);
	void SetFlags();
	void SetStyle();
	void Terminate();
	void Render();

	void StartFrame();
	void EndFrame();

	bool PathBox(const char* label, std::filesystem::path& path, char* pathBuffer, ImVec2 size_arg);
	bool FavoriteButton(const char* label, bool isFavorite);
	bool FolderNode(const char* label, ImTextureID icon, bool& clicked);
	bool FileIcon(const char* label, bool isSelected, ImTextureID icon, ImVec2 size);
}
#pragma once
#include <string>
#include <filesystem>
#include <vector>

#include "Twisted/Application/Window.h"

namespace Twisted::Native
{
	std::string OpenFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter);
	std::string OpenFolderDialog(Window& window);
	std::filesystem::path SaveFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter);

	bool ShowConfirmDialog(HWND owner, const std::wstring& message, const std::wstring& title);
}
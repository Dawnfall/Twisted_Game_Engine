#pragma once
#include "AppCore.h"
#include <string>
#include <filesystem>
#include <Windows.h>
#include <vector>

#include "Window.h"

namespace Twisted::Native
{
	TWISTED_API std::filesystem::path OpenFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter);
	TWISTED_API std::filesystem::path OpenFolderDialog(Window& window);
	TWISTED_API std::filesystem::path SaveFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter);

	TWISTED_API bool ShowConfirmDialog(Window& window, const std::wstring& message, const std::wstring& title);
}


#ifndef _WIN32
#include "Twisted/Windowing/WIN32/Win32Utils.h"

namespace Twisted::Windows
{
    std::filesystem::path OpenFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
    {
        return {};
    }

    std::filesystem::path OpenFolderDialog(Window& window)
    {
        return {};
    }

    std::filesystem::path SaveFileDialog(Window& window, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
    {
        return {};
    }

    bool ShowConfirmDialog(Window& window, const std::wstring& message, const std::wstring& title)
    {
        return false;
    }
}
#endif

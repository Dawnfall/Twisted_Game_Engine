#ifndef _WIN32
#include "WIN32/NativeUtils_Win.h"
#include <cstdio>
#include <cstdint>

namespace Twisted::Windows
{

static std::string wideToUtf8(const std::wstring& ws)
{
    std::string out;
    out.reserve(ws.size());
    for (wchar_t wc : ws)
    {
        uint32_t cp = static_cast<uint32_t>(wc);
        if (cp < 0x80)
            out += static_cast<char>(cp);
        else if (cp < 0x800) {
            out += static_cast<char>(0xC0 | (cp >> 6));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            out += static_cast<char>(0xE0 | (cp >> 12));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        } else {
            out += static_cast<char>(0xF0 | (cp >> 18));
            out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
            out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (cp & 0x3F));
        }
    }
    return out;
}

enum class Backend { Zenity, KDialog, None };

static Backend detectBackend()
{
    if (system("command -v zenity  > /dev/null 2>&1") == 0) return Backend::Zenity;
    if (system("command -v kdialog > /dev/null 2>&1") == 0) return Backend::KDialog;
    return Backend::None;
}

static std::string runCommand(const std::string& cmd)
{
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};
    char buf[4096];
    std::string result;
    while (fgets(buf, sizeof(buf), pipe))
        result += buf;
    pclose(pipe);
    if (!result.empty() && result.back() == '\n')
        result.pop_back();
    return result;
}

static std::string zenityFilters(const std::vector<std::pair<std::wstring, std::wstring>>& filter)
{
    std::string out;
    for (const auto& f : filter)
        out += " --file-filter='" + wideToUtf8(f.first) + " | " + wideToUtf8(f.second) + "'";
    return out;
}

static std::string kdialogFilter(const std::vector<std::pair<std::wstring, std::wstring>>& filter)
{
    std::string out;
    for (size_t i = 0; i < filter.size(); ++i)
    {
        if (i > 0) out += '\n';
        out += wideToUtf8(filter[i].second) + '|' + wideToUtf8(filter[i].first);
    }
    return out;
}

std::filesystem::path OpenFileDialog(Window& /*window*/, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
{
    switch (detectBackend())
    {
    case Backend::Zenity:
        return runCommand("zenity --file-selection" + zenityFilters(filter) + " 2>/dev/null");
    case Backend::KDialog:
        return runCommand("kdialog --getopenfilename . '" + kdialogFilter(filter) + "' 2>/dev/null");
    default:
        return {};
    }
}

std::filesystem::path OpenFolderDialog(Window& /*window*/)
{
    switch (detectBackend())
    {
    case Backend::Zenity:
        return runCommand("zenity --file-selection --directory 2>/dev/null");
    case Backend::KDialog:
        return runCommand("kdialog --getexistingdirectory . 2>/dev/null");
    default:
        return {};
    }
}

std::filesystem::path SaveFileDialog(Window& /*window*/, const std::vector<std::pair<std::wstring, std::wstring>>& filter)
{
    switch (detectBackend())
    {
    case Backend::Zenity:
        return runCommand("zenity --file-selection --save --confirm-overwrite" + zenityFilters(filter) + " 2>/dev/null");
    case Backend::KDialog:
        return runCommand("kdialog --getsavefilename . '" + kdialogFilter(filter) + "' 2>/dev/null");
    default:
        return {};
    }
}

bool ShowConfirmDialog(Window& /*window*/, const std::wstring& message, const std::wstring& title)
{
    std::string msg = wideToUtf8(message);
    std::string ttl = wideToUtf8(title);
    switch (detectBackend())
    {
    case Backend::Zenity:
        return system(("zenity --question --text='" + msg + "' --title='" + ttl + "' 2>/dev/null").c_str()) == 0;
    case Backend::KDialog:
        return system(("kdialog --yesno '" + msg + "' --title '" + ttl + "' 2>/dev/null").c_str()) == 0;
    default:
        return false;
    }
}

}
#endif

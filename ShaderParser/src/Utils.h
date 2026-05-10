#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

inline std::string ReadFile(const fs::path& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return {};
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

inline std::string_view TrimLeft(std::string_view s)
{
    size_t i = 0;
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t'))
        ++i;
    return s.substr(i);
}

inline std::string_view TrimRight(std::string_view s)
{
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t'))
        s.remove_suffix(1);
    return s;
}

inline std::string_view Trim(std::string_view s)
{
    return TrimRight(TrimLeft(s));
}

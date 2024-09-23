#pragma once

#include "pch.h"

struct Resource {
    std::string fileName;
    std::string content;
};

namespace Twisted::Utils
{
	template<typename T>
	bool contains(const std::vector<T>& vec, const T& value)
	{
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}

	template<typename T>
	void removeValue(std::vector<T>& vec, const T& value)
	{
		vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
	}

	std::string ReadFileContent(const std::string& filePath);
	std::vector<std::filesystem::directory_entry> GetFilesInFolder(const std::string& folderPath);
	std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);











}
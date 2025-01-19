#pragma once
#include "AppCore.h"
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace Twisted::Utils
{
	/// <summary>
	/// Checks if a given path is a folder
	/// </summary>
	/// <param name="path">path to check</param>
	/// <returns>true if the path is a folder, false otherwise</returns>
	/// 
	bool IsFolder(const fs::path& path);

	bool IsExistingFolder(const fs::path& path);

	bool IsFile(const fs::path& path);

	size_t FileSize(const fs::path& path);

	time_t DateModified(const fs::path& path);

	std::vector<fs::path> SubEntries(const fs::path& path);

	std::vector<fs::path> SubFiles(const fs::path& path);

	std::vector<fs::path> SubFolders(const fs::path& path);

	std::vector<fs::path> GetAllDrives();

	/// <summary>
	/// Finds all files in a folder recursively
	/// </summary>
	/// <param name="folderPath">root folder to search</param>
	/// <returns>vector of directory entries recursively in the root folder</returns>
	
	std::vector<fs::directory_entry> GetFilesInFolder(const std::string& folderPath);

	bool CreateFolder(const fs::path& folderPath);

	bool CreateNewFile(const fs::path& filePath);

	std::string ReadFileContent(const fs::path& filePath);

	std::filesystem::path CombinePaths(const std::string& absolutePath, const std::string& folderName);

	bool IsEmptyDirectory(const fs::path& path);

	std::vector<std::filesystem::path> LoadFiles(const fs::path& folderPath, const std::string& extension);

	bool WriteBufferToFile(const std::filesystem::path& filepath, const char* buffer, std::size_t size);

	bool ReadBufferFromFile(const std::filesystem::path& filepath, char* buffer, std::size_t size);

	//// Method to write buffer to a binary file
	//bool WriteBufferToFile(const std::string& filePath, const Buffer& buffer);

	//// Method to read buffer from a binary file
	//bool ReadBufferFromFile(const std::string& filePath, Buffer& buffer);
}
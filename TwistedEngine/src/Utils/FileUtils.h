#pragma once
#include "twistedpch.h"
#include "Debug/Logger.h"
#include "Utils/Buffer.h"
#include "AppCore.h"

namespace fs = std::filesystem;

namespace Twisted::Utils
{
	/// <summary>
	/// Checks if a given path is a folder
	/// </summary>
	/// <param name="path">path to check</param>
	/// <returns>true if the path is a folder, false otherwise</returns>
	/// 
	TWISTED_API bool IsFolder(const fs::path& path);

	TWISTED_API bool IsExistingFolder(const fs::path& path);

	TWISTED_API bool IsFile(const fs::path& path);

	TWISTED_API size_t FileSize(const fs::path& path);

	TWISTED_API time_t DateModified(const fs::path& path);

	TWISTED_API std::vector<fs::path> SubEntries(const fs::path& path);

	TWISTED_API std::vector<fs::path> SubFiles(const fs::path& path);

	TWISTED_API std::vector<fs::path> SubFolders(const fs::path& path);

	TWISTED_API std::vector<fs::path> GetAllDrives();
	/// <summary>
	/// Finds all files in a folder recursively
	/// </summary>
	/// <param name="folderPath">root folder to search</param>
	/// <returns>vector of directory entries recursively in the root folder</returns>
	TWISTED_API std::vector<fs::directory_entry> GetFilesInFolder(const std::string& folderPath);

	TWISTED_API bool CreateFolder(const fs::path& folderPath);

	TWISTED_API bool CreateNewFile(const fs::path& filePath);

	TWISTED_API std::string ReadFileContent(const fs::path& filePath);

	TWISTED_API std::filesystem::path CombinePaths(const std::string& absolutePath, const std::string& folderName);

	TWISTED_API bool IsEmptyDirectory(const fs::path& path);

	TWISTED_API std::vector<std::filesystem::path> LoadFiles(const fs::path& folderPath, const std::string& extension);
	//// Method to write buffer to a binary file
	//bool WriteBufferToFile(const std::string& filePath, const Buffer& buffer);

	//// Method to read buffer from a binary file
	//bool ReadBufferFromFile(const std::string& filePath, Buffer& buffer);
}
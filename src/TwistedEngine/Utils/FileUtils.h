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
	bool TWISTED_API IsFolder(const fs::path& path);

	bool TWISTED_API IsFile(const fs::path& path);

	size_t TWISTED_API FileSize(const fs::path& path);

	time_t TWISTED_API DateModified(const fs::path& path);

	std::vector<fs::path> TWISTED_API SubEntries(const fs::path& path);

	std::vector<fs::path> TWISTED_API SubFiles(const fs::path& path);

	std::vector<fs::path> TWISTED_API SubFolders(const fs::path& path);

	std::vector<fs::path> TWISTED_API GetAllDrives();

	bool TWISTED_API IsExisting(const fs::path& path);
	
	std::vector<fs::directory_entry> TWISTED_API GetFilesInFolder(const std::string& folderPath);

	bool TWISTED_API CreateFolder(const fs::path& folderPath);

	bool TWISTED_API CreateNewFile(const fs::path& filePath);

	std::string TWISTED_API ReadFileContent(const fs::path& filePath);

	std::filesystem::path TWISTED_API CombinePaths(const std::string& absolutePath, const std::string& folderName);

	bool TWISTED_API IsEmptyDirectory(const fs::path& path);

	std::vector<std::filesystem::path> TWISTED_API LoadFiles(const fs::path& folderPath, const std::string& extension);

	bool TWISTED_API EnsureFileExists(const std::filesystem::path& folder, const std::filesystem::path& filename);

	std::vector<fs::path> TWISTED_API ListAllRelativeFiles(const fs::path& folderPath, const std::vector<fs::path>& extensions = {});
	
	bool TWISTED_API DeleteAtPath(const std::filesystem::path& path);

	//// Method to write buffer to a binary file
	//bool WriteBufferToFile(const std::string& filePath, const Buffer& buffer);

	//// Method to read buffer from a binary file
	//bool ReadBufferFromFile(const std::string& filePath, Buffer& buffer);
}


#pragma once
#include "twistedpch.h"
#include "Debug/Logger.h"
#include "Utils/Buffer.h"

namespace Twisted::Utils
{
	/// <summary>
	/// Checks if a given path is a folder
	/// </summary>
	/// <param name="path">path to check</param>
	/// <returns>true if the path is a folder, false otherwise</returns>
	/// 
	bool IsFolder(const std::string& path);

	/// <summary>
	/// Finds all files in a folder recursively
	/// </summary>
	/// <param name="folderPath">root folder to search</param>
	/// <returns>vector of directory entries recursively in the root folder</returns>
	std::vector<std::filesystem::directory_entry> GetFilesInFolder(const std::string& folderPath);

	std::string ReadFileContent(const std::string& filePath);

	//// Method to write buffer to a binary file
	//bool WriteBufferToFile(const std::string& filePath, const Buffer& buffer);

	//// Method to read buffer from a binary file
	//bool ReadBufferFromFile(const std::string& filePath, Buffer& buffer);
}
#include "twistedpch.h"
#include "FileUtils.h"

namespace Twisted::Utils
{
	bool IsFolder(const std::string& path)
	{
		return std::filesystem::is_directory(path);
	}

	std::vector<std::filesystem::directory_entry> GetFilesInFolder(const std::string& folderPath)
	{
		std::vector<std::filesystem::directory_entry> files;
		if (!Utils::IsFolder(folderPath))
		{
			TWISTED_WARN("Not a folder: " + folderPath);
			return files;
		}

		for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
		{
			if (entry.is_regular_file())
				files.push_back(entry);
		}
		return files;
	}

	bool CreateFolder(const std::filesystem::path& folderPath)
	{
		try
		{
			if (std::filesystem::create_directories(folderPath))
			{
				std::cout << "Folder created: " << folderPath << std::endl;
				return true;
			}
			else
			{
				std::cerr << "Folder already exists or failed to create: " << folderPath << std::endl;
				return false;
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			std::cerr << "Filesystem error: " << e.what() << std::endl;
			return false;
		}
	}


	std::string ReadFileContent(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			TWISTED_WARN(std::format("Failed to open file: {0}", filePath.string()));
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	std::filesystem::path CombinePaths(const std::string& absolutePath, const std::string& folderName)
	{
		std::filesystem::path path(absolutePath);
		path /= folderName;
		return path;
	}

	bool IsExistingFolder(const std::filesystem::path& path)
	{
		return std::filesystem::exists(path) && std::filesystem::is_directory(path);
	}

	std::vector<std::filesystem::path> LoadFiles(const std::filesystem::path& folderPath, const std::string& extension)
	{
		std::vector<std::filesystem::path> files;

		try
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
			{
				if (entry.is_regular_file() && (extension.empty() || entry.path().extension() == extension))
				{
					files.emplace_back(entry.path());
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			// Handle the error, e.g., log it
			std::cerr << "Filesystem error: " << e.what() << std::endl;
		}

		return files;
	}


	//bool WriteBufferToFile(const std::string& filePath, const Buffer& buffer)
	//{
	//	std::ofstream outFile(filePath, std::ios::binary);
	//	if (!outFile)
	//	{
	//		std::cerr << "Failed to open file for writing: " << filePath << std::endl;
	//		return false;
	//	}
	//	outFile.write(buffer.Data, buffer.Size);
	//	outFile.close();
	//	return true;
	//}

	//bool ReadBufferFromFile(const std::string& filePath, Buffer& buffer)
	//{
	//	std::ifstream inFile(filePath, std::ios::binary | std::ios::ate);
	//	if (!inFile)
	//	{
	//		std::cerr << "Failed to open file for reading: " << filePath << std::endl;
	//		return false;
	//	}
	//	std::streamsize fileSize = inFile.tellg();
	//	inFile.seekg(0, std::ios::beg);

	//	buffer.Data = new char[fileSize];
	//	buffer.Size = static_cast<size_t>(fileSize);

	//	if (!inFile.read(buffer.Data, fileSize))
	//	{
	//		std::cerr << "Failed to read file: " << filePath << std::endl;
	//		delete[] buffer.Data;
	//		buffer.Data = nullptr;
	//		buffer.Size = 0;
	//		return false;
	//	}
	//	inFile.close();
	//	return true;
	//}
}

#include "FileUtils.h"
#include "AppCore.h"
#include "Buffer.h"

#include <filesystem>
#include <fstream>
#include <chrono>
namespace Twisted::Utils
{
	bool IsFolder(const fs::path& path)
	{
		std::error_code ec;
		bool isDir = fs::is_directory(path, ec);
		if (ec || !isDir)
			return false;
		return true;
	}

	bool IsFile(const fs::path& path)
	{
		std::error_code ec;
		bool isFile = fs::is_regular_file(path, ec);
		if (ec || !isFile)
			return false;
		return true;
	}

	size_t FileSize(const fs::path& path)
	{
		std::error_code ec;
		size_t size = fs::file_size(path, ec);
		if (ec)
			return 0;
		return size;
	}

	time_t DateModified(const fs::path& path)
	{
		auto ftime = std::filesystem::last_write_time(path);
		auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			ftime - std::filesystem::file_time_type::clock::now()
			+ std::chrono::system_clock::now()
		);
		return std::chrono::system_clock::to_time_t(sctp);
	}

	std::vector<fs::path> SubEntries(const fs::path& path)
	{
		std::vector<fs::path> subEntries;
		std::error_code ec;
		for (auto entry : fs::directory_iterator(path, ec))
			subEntries.emplace_back(entry);

		return subEntries;
	}

	std::vector<fs::path> SubFiles(const fs::path& path)
	{
		std::vector<fs::path> subEntries;
		std::error_code ec;
		for (auto entry : fs::directory_iterator(path, ec))
			if (IsFile(entry))
				subEntries.emplace_back(entry);

		return subEntries;
	}

	std::vector<fs::path> SubFolders(const fs::path& path)
	{
		std::vector<fs::path> subEntries;
		std::error_code ec;
		for (auto entry : fs::directory_iterator(path, ec))
			if (IsFolder(entry.path()))
				subEntries.emplace_back(entry);

		return subEntries;
	}

	std::vector<fs::path> GetAllDrives()
	{
		std::vector<fs::path> drives;

		char drive = 'A';
		while (drive <= 'Z')
		{
			std::string drivePath = std::string(1, drive) + ":\\";
			if (fs::exists(drivePath))
			{
				drives.push_back(drivePath);
			}
			++drive;
		}
		return drives;
	}

	std::vector<fs::directory_entry> GetFilesInFolder(const std::string& folderPath)
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

	bool CreateFolder(const fs::path& folderPath) //corrected
	{
		std::error_code ec;
		if (fs::create_directories(folderPath, ec) && !ec)
			return true;
		return false;
	}

	bool CreateNewFile(const fs::path& filePath) //corrected
	{
		std::error_code ec;
		if (fs::exists(filePath, ec) || ec)
			return false; // File already exists or error checking

		std::ofstream file(filePath);
		if (!file.is_open())
			return false;
		file.close();
		return true;
	}

	std::string ReadFileContent(const fs::path& filePath)
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

	bool IsExisting(const fs::path& path)
	{
		std::error_code ec;
		return fs::exists(path, ec) && !ec;
	}

	bool IsEmptyDirectory(const fs::path& path)
	{
		std::error_code ec;
		if (fs::is_directory(path, ec) && !ec)
			if (fs::is_empty(path, ec) && !ec)
				return true;
		return false;
	}

	std::vector<fs::path> LoadFiles(const fs::path& folderPath, const std::string& extension)
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
			TWISTED_ERROR("Filesystem error: ");
		}

		return files;
	}

	bool WriteBufferToFile(const std::filesystem::path& filepath, const Buffer& buffer)
	{
		std::error_code ec;
		std::ofstream file(filepath, std::ios::out | std::ios::binary);
		if (!file)
		{
			TWISTED_WARN("Failed to open file for writing: ");
			return false;
		}

		size_t dataSize = buffer.GetDataSize();
		file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
		file.write(buffer.GetData(), dataSize);
		file.close();

		if (ec || !file.good())
		{
			TWISTED_WARN("Error occurred while writing to file: ");
			return false;
		}

		return true;
	}

	Buffer ReadBufferFromFile(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		if (!file)
		{
			TWISTED_WARN("Failed to open file for reading: ");
			return Buffer(0);
		}

		size_t dataSize = 0;
		file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));

		Buffer buffer(dataSize);
		file.read(buffer.DataPointer(), dataSize);
		file.close();

		if (!file.good())
		{
			TWISTED_WARN("Error occurred while reading from file: ");
			return Buffer(0);
		}

		return buffer;
	}

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
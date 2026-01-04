
#include "AppCore.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"

#include <filesystem>
#include <fstream>
#include <chrono>
#include <corecrt.h>
#include <vector>
#include <system_error>
#include <sstream>
#include <string>
#include <algorithm>
#include <format>

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
		for (auto& entry : fs::directory_iterator(path, ec))
			subEntries.emplace_back(entry);

		return subEntries;
	}

	std::vector<fs::path> SubFiles(const fs::path& path)
	{
		std::vector<fs::path> subEntries;
		std::error_code ec;
		for (auto& entry : fs::directory_iterator(path, ec))
			if (IsFile(entry))
				subEntries.emplace_back(entry);

		return subEntries;
	}

	std::vector<fs::path> SubFolders(const fs::path& path)
	{
		std::vector<fs::path> subEntries;
		std::error_code ec;
		for (auto& entry : fs::directory_iterator(path, ec))
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
			TWISTED_WARN("Not a folder: {}", folderPath);
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
		bool result = fs::create_directories(folderPath, ec);

		if (ec)
		{
			TWISTED_WARN("Cannot create folder at path: {} , error: {}", folderPath.string(), ec.message());
		}
		return result;
	}

	bool CreateNewFile(const fs::path& filePath) //corrected
	{
		std::error_code ec;
		bool doExists = fs::exists(filePath, ec);
		if (ec)
		{
			TWISTED_WARN("Cannot create file at path: {} , error: ", filePath.string(), ec.message());
			return false; // File already exists or error checking
		}
		if (doExists)
		{
			TWISTED_INFO("File already exists: {}", filePath.string());
			return true;
		}

		try
		{
			std::ofstream file(filePath);
			if (!file.is_open())
				return false;
			file.close();
			return true;

		}
		catch (...)
		{
			TWISTED_WARN("Error creating file: {}", filePath.string());
			return false;
		}

	}

	std::string ReadFileContent(const fs::path& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			TWISTED_WARN(std::format("Failed to open file: {}", filePath.string()));
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

	bool EnsureFileExists(const std::filesystem::path& folder, const std::filesystem::path& filename)
	{
		std::error_code ec;
		std::filesystem::create_directories(folder, ec); // No throw, error in ec

		std::filesystem::path filePath = folder / filename;
		if (std::filesystem::exists(filePath, ec))
			return true;

		std::ofstream ofs(filePath);
		return ofs.good();
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
		catch ([[maybe_unused]] const std::filesystem::filesystem_error& e)
		{
			TWISTED_ERROR("Filesystem error: ",e.what());
		}

		return files;
	}



	// Add this method
	std::vector<std::filesystem::path> TWISTED_API ListAllRelativeFiles(const std::filesystem::path& folderPath, const std::vector<std::filesystem::path>& extensions)
	{
		std::vector<std::filesystem::path> result;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
		{
			if (entry.is_regular_file() && (extensions.empty() || std::find(extensions.begin(), extensions.end(), entry.path().extension()) != extensions.end()))
			{
				result.push_back(std::filesystem::relative(entry.path(), folderPath));
			}
		}
		return result;
	}

	bool DeleteAtPath(const std::filesystem::path& path)
	{
		std::error_code ec;
		std::filesystem::remove(path, ec); // Use error_code to avoid exceptions
		if (ec)
			return false;
		return true;
	}
}


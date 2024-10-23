#include "pch.h"
#include "Utils.h"
#include "Debug/Logger.h"

namespace Twisted::Utils
{
	std::string ReadFileContent(const std::string& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			TWISTED_WARN(std::format("Failed to open file: {0}", filePath));
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	std::string ExchangeFileContentsWithOther(std::string& str, const std::string& toBeExchanged, const std::string& changed)
	{
		std::regex insertRegex(toBeExchanged);
		std::smatch match;

		while (std::regex_search(str, match, insertRegex))
		{
			std::string fullMatch = match[0];
			str.replace(match.position(0), match.length(0), changed);
		}
		return str;
	}

	/// <summary>
	/// Splits a string by a delimiter
	/// </summary>
	/// <param name="str">input string</param>
	/// <param name="delimiter">delimiter string</param>
	/// <returns>vector of split strings</returns>
	std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> tokens;
		size_t start = 0;
		size_t end = str.find(delimiter);

		while (end != std::string::npos) {
			tokens.push_back(str.substr(start, end - start));
			start = end + delimiter.length();
			end = str.find(delimiter, start);
		}

		tokens.push_back(str.substr(start, end));
		return tokens;
	}

	/// <summary>
	/// Finds all files in a folder recursively
	/// </summary>
	/// <param name="folderPath">root folder to search</param>
	/// <returns>vector of directory entries recursively in the root folder</returns>
	std::vector<std::filesystem::directory_entry> GetFilesInFolder(const std::string& folderPath)
	{
		std::vector<std::filesystem::directory_entry> files;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
		{
			if (entry.is_regular_file())
				files.push_back(entry);
		}
		return files;
	}
}
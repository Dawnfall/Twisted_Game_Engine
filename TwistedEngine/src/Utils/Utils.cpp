#include "twistedpch.h"
#include "Utils.h"
#include "FileUtils.h"
#include "Debug/Logger.h"

namespace Twisted::Utils
{
	
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

	std::string ExchangeStringContentsWithOther(std::string& str, const std::string& toBeExchanged, const std::string& changed)
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
}
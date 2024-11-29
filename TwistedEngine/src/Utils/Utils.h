#pragma once

#include "twistedpch.h"
#include "AppCore.h"

struct TWISTED_API Resource {
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

	TWISTED_API std::string ExchangeStringContentsWithOther(std::string& str, const std::string& toBeExchanged, const std::string& changed);

	TWISTED_API std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);

	TWISTED_API std::string GenerateGUID();

	TWISTED_API void StringToArray(const std::string& str, char* buffer, size_t bufferSize);








}
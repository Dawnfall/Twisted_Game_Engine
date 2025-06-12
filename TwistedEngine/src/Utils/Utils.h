#pragma once

#include "AppCore.h"
#include <vector>
#include <string>

struct Resource 
{
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

	template<typename T>
	void MoveItemInVector(std::vector<T>& vec, size_t from, size_t to)
	{
		if (from == to || from >= vec.size() || to > vec.size())
			return;

		auto item = vec[from];
		vec.erase(vec.begin() + from);
		// If moving forward, the erase shifts indices down, so adjust 'to'
		if (to > from) --to;
		vec.insert(vec.begin() + to, item);
	}

	std::string ExchangeStringContentsWithOther(std::string& str, const std::string& toBeExchanged, const std::string& changed);

	std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);

	std::string GenerateGUID();

	void StringToArray(const std::string& str, char* buffer, size_t bufferSize);

}
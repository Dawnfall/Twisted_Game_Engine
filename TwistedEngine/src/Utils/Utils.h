#pragma once

#include "twistedpch.h"

struct Resource {
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
	void removeValue(std::vector<T>& vec, const T& value)
	{
		vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
	}

	std::string ExchangeStringContentsWithOther(std::string& str, const std::string& toBeExchanged, const std::string& changed);

	std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);











}
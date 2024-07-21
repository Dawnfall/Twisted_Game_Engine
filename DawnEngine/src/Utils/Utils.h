#pragma once

#include <vector>
#include <algorithm>

namespace Dawn::Utils
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



}
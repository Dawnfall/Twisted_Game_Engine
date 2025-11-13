#pragma once

#include "AppCore.h"
#include <vector>
#include <string>
#include <sstream>
//struct Resource 
//{
//    std::string fileName;
//    std::string content;
//};

namespace Twisted::Utils
{
	template<typename T>
	bool contains(const std::vector<T>& vec, const T& value)
	{
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}

	template<typename T>
	bool RemoveElement(std::vector<T>& vec,const T& el)
	{
		auto it = std::find(vec.begin(), vec.end(), el);
		if (it != vec.end())
		{
			vec.erase(it);
			return true;
		}
		return false;
	}

	template<typename T>
	void MoveItemInVector(std::vector<T>& vec, size_t from, size_t to)
	{
		if (from == to || from >= vec.size() || to >= vec.size())
			return;

		T item = std::move(vec[from]);
		vec.erase(vec.begin() + from);

		vec.insert(vec.begin() + to, std::move(item));
	}

	template<typename T>
	std::string GetObjTypeName(T* obj)
	{
		if (!obj) return "Null";

		std::string name = typeid(*obj).name();

#if defined(__GNUC__) || defined(__clang__)
		int status = 0;
		char* demangled = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
		if (status == 0 && demangled)
		{
			name = demangled;
			free(demangled);
		}
#endif

		// Strip namespaces (take substring after last ::)
		size_t pos = name.rfind("::");
		if (pos != std::string::npos)
			name = name.substr(pos + 2);

		return name;
	}

	TWISTED_API std::string ExchangeStringContentsWithOther(std::string& str, const std::string& toBeExchanged, const std::string& changed);

	TWISTED_API std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);

	TWISTED_API void StringToArray(const std::string& str, char* buffer, size_t bufferSize);

	// Helper to build a string from variadic arguments
	template<typename... Args>
	std::string buildString(Args&&... args)
	{
		std::ostringstream oss;
		(oss << ... << args); // fold expression: simple concatenation
		return oss.str();
	}
}


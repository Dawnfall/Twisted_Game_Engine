#pragma once
#include "Utils/Buffer.h"

namespace Twisted
{
	template<typename T>
	Buffer Serialize(const T& obj) { static_assert(std::is_same<T, void>::value, "This template must be specialized."); }

	template<typename T>
	T Deserialize(const Buffer& buffer) { static_assert(std::is_same<T, void>::value, "This template must be specialized."); }
}
#pragma once

#include "BinSerializer.h"

namespace Twisted
{
	template<typename T>
	void Serialize(const T& obj, BinSerializer& buffer) = delete;
	template<typename T>
	void Deserialize(T& obj, BinSerializer& buffer) = delete;
	template<typename T>
	void PreSerialize(const T& obj, BinSerializer& buffer) = delete;
	template<typename T>
	void PostDeserialize(T& obj, BinSerializer& buffer) = delete;
}
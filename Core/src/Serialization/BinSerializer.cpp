#include "Serialization/BinSerializer.h"

#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>

namespace Twisted
{
	BinSerializer::BinSerializer(const std::vector<char>& data, size_t position) :
		m_buffer(data),
		m_readCurrIndex(position)
	{
	}
	BinSerializer::BinSerializer(const BinSerializer& other) :
		m_readCurrIndex(other.m_readCurrIndex)
	{
		m_buffer.resize(other.m_readCurrIndex);
		std::copy_n(other.m_buffer.begin(), other.m_readCurrIndex, m_buffer.begin());
	}

	BinSerializer::BinSerializer(BinSerializer&& other) :
		m_readCurrIndex(other.m_readCurrIndex),
		m_buffer(std::move(other.m_buffer))
	{
		other.m_readCurrIndex = 0;
	}

	BinSerializer& BinSerializer::operator=(const BinSerializer& other)
	{
		if (this != &other)
		{
			m_readCurrIndex = other.m_readCurrIndex;
			m_buffer.resize(other.m_readCurrIndex);
			std::copy_n(other.m_buffer.begin(), other.m_readCurrIndex, m_buffer.begin());
		}
		return *this;
	}
	BinSerializer& BinSerializer::operator=(BinSerializer&& other)
	{
		if (this != &other) // Self-assignment check
		{
			m_readCurrIndex = other.m_readCurrIndex;
			m_buffer = std::move(other.m_buffer);
			other.m_readCurrIndex = 0; // Reset moved-from object
		}
		return *this;
	}


	bool BinSerializer::SaveToFile(const std::filesystem::path& path)
	{
		std::ofstream outFile(path, std::ios::binary);
		if (!outFile)
			return false; // Optionally handle error
		outFile.write(m_buffer.data(), m_buffer.size());
		return true;
	}

	bool BinSerializer::LoadFromFile(const std::filesystem::path& path)
	{
		std::ifstream inFile(path, std::ios::binary | std::ios::ate);
		if (!inFile)
			return false; // Optionally handle error

		std::streamsize size = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		m_buffer.resize(static_cast<size_t>(size));
		if (size > 0)
			inFile.read(m_buffer.data(), size);

		m_readCurrIndex = 0;
		return true;
	}


}


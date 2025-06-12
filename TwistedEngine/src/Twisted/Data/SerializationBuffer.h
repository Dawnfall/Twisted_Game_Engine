#pragma once

#include "AppCore.h"
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <type_traits>

template<typename T>
constexpr bool is_trivially_copyable_v = std::is_trivially_copyable<T>::value;

template<typename T>
constexpr bool always_false_v = false;

template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template<typename T>
constexpr bool is_vector_v = is_vector<T>::value;

namespace Twisted
{
	class BinSerializer
	{
	public:
		void EnsureCapacity(size_t requiredSize)
		{
			size_t emptySize = m_buffer.capacity() - m_buffer.size();

			if (emptySize < requiredSize)
				Resize((m_buffer.size() + requiredSize) * 2);
		}

		void Resize(size_t newSize)
		{
			m_buffer.reserve(newSize);
		}

		template<typename T>
		void Write(const T& data)
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				EnsureCapacity(sizeof(T));
				const char* dataPtr = reinterpret_cast<const char*>(&data);
				m_buffer.insert(m_buffer.end(), dataPtr, dataPtr + sizeof(T));
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				Write(data.size()); // write string length
				m_buffer.insert(m_buffer.end(), data.begin(), data.end()); // write raw characters
			}
			else if constexpr (is_vector_v<T>)
			{
				Write(data.size()); // write vector size
				for (const auto& element : data)
					Write(element); // recursively write each element
			}
			else
			{
				static_assert(always_false_v<T>, "Unsupported type in Write()");
			}
		}

		template<typename T>
		T Read()
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				if (m_readCurrIndex + sizeof(T) > m_buffer.size())
					throw std::runtime_error("Read out of bounds");

				T value;
				std::memcpy(&value, m_buffer.data() + m_readCurrIndex, sizeof(T));
				m_readCurrIndex += sizeof(T);
				return value;
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				size_t size = Read<size_t>();
				if (m_readCurrIndex + size > m_buffer.size())
					throw std::runtime_error("Read<std::string> out of bounds");

				std::string str(m_buffer.begin() + m_readCurrIndex, m_buffer.begin() + m_readCurrIndex + size);
				m_readCurrIndex += size;
				return str;
			}
			else if constexpr (is_vector_v<T>)
			{
				using ElemT = typename T::value_type;
				size_t size = Read<size_t>();
				T vec(size);
				for (auto& element : vec)
					element = Read<ElemT>();
				return vec;
			}
			else
			{
				static_assert(always_false_v<T>, "Unsupported type in Read()");
			}
		}

		bool SaveToFile(const std::filesystem::path& path)
		{
			std::ofstream outFile(path, std::ios::binary);
			if (!outFile)
				return false; // Optionally handle error
			outFile.write(m_buffer.data(), m_buffer.size());
			return true;
		}

		bool LoadFromFile(const std::filesystem::path& path)
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

	private:
		size_t m_readCurrIndex = 0;
		std::vector<char> m_buffer;
	};
}
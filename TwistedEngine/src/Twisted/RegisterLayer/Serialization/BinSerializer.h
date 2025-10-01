#pragma once

//TODO: byte order normalization... little/big endian

#include "AppCore.h"
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <type_traits>
#include <unordered_map>

template<typename T>
constexpr bool is_trivially_copyable_v = std::is_trivially_copyable<T>::value;

template<typename T>
constexpr bool always_false_v = false;

template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template<typename T>
struct is_unordered_map : std::false_type {};

template<typename K, typename V, typename... Args>
struct is_unordered_map<std::unordered_map<K, V, Args...>> : std::true_type {};

template<typename T>
inline constexpr bool is_unordered_map_v = is_unordered_map<T>::value;

template<typename T>
constexpr bool is_vector_v = is_vector<T>::value;

namespace Twisted
{
	class BinSerializer;

	template<typename T>
	T readFromBuffer(BinSerializer& buffer) = delete;

	// Concepts for custom free functions
	template<typename T>
	concept HasWriteToBuffer = requires(const T & obj, BinSerializer & buf) {
		writeToBuffer(obj, buf);
	};

	template<typename T>
	concept HasReadFromBuffer = requires(BinSerializer & buf) {
		{ readFromBuffer<T>(buf) } -> std::same_as<T>;
	};

	class TWISTED_API BinSerializer
	{
	public:
		BinSerializer() = default;
		BinSerializer(const std::vector<char>& data, size_t position);
		BinSerializer(const BinSerializer& other);
		BinSerializer(BinSerializer&& other);
		BinSerializer& operator=(const BinSerializer& other);
		BinSerializer& operator=(BinSerializer&& other);

		void EnsureCapacity(size_t requiredSize)
		{
			size_t emptySize = m_buffer.capacity() - m_buffer.size();

			if (emptySize < requiredSize)
				Resize((m_buffer.size() + requiredSize) * 2);
		}

		void Resize(size_t newSize) { m_buffer.reserve(newSize); }

		size_t GetSize()const { return m_buffer.size(); }
		size_t GetCapacity()const { return m_buffer.capacity(); }
		size_t GetCurrentIndex()const { return m_readCurrIndex; }

		void IncreaseIndex(size_t delta) { m_readCurrIndex += delta; }
		std::vector<char>& GetData() { return m_buffer; }

		template<typename T>
		void WriteBytes(const T* data, size_t count)
		{
			static_assert(std::is_trivially_copyable_v<T>, "WriteBytes requires trivially copyable types");

			size_t sizeInBytes = count * sizeof(T);

			size_t oldSize = m_buffer.size();
			EnsureCapacity(oldSize + sizeInBytes);  // ensure capacity, not size

			m_buffer.resize(oldSize + sizeInBytes); // actually increase size
			std::memcpy(m_buffer.data() + oldSize, data, sizeInBytes);
		}

		template<typename T>
		void ReadBytes(T* outData, size_t count)
		{
			static_assert(std::is_trivially_copyable_v<T>, "ReadBytes requires trivially copyable types");

			size_t sizeInBytes = count * sizeof(T);
			if (m_readCurrIndex + sizeInBytes > m_buffer.size())
			{
				throw std::runtime_error("BinSerializer: Read out of bounds");
			}
			std::memcpy(outData, m_buffer.data() + m_readCurrIndex, sizeInBytes);
			m_readCurrIndex += sizeInBytes;
		}

		template<typename T>
		void Write(const T& obj) {
			if constexpr (HasWriteToBuffer<T>) {
				writeToBuffer(obj, *this);  // custom free function
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				Write<size_t>(obj.size());
				EnsureCapacity(obj.size());
				m_buffer.insert(m_buffer.end(), obj.begin(), obj.end());
			}
			else if constexpr (is_vector_v<T>)
			{
				Write<size_t>(obj.size());
				for (const auto& element : obj)
					Write(element); // recursively write elements
			}
			else if constexpr (is_unordered_map_v<T>)
			{
				Write<size_t>(obj.size());
				for (const auto& [key, value] : obj)
				{
					Write(key);
					Write(value);
				}
			}
			else if constexpr (std::is_trivially_copyable_v<T>) {
				WriteBytes(&obj, 1);        // fallback for trivially copyable types (includes enums)
			}
			else {
				static_assert(sizeof(T) == 0, "No way to serialize this type!");
			}
		}

		template<typename T>
		T Read() {
			if constexpr (HasReadFromBuffer<T>)
			{
				return readFromBuffer<T>(*this);
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				size_t size = Read<size_t>();
				std::string str(size, '\0');
				ReadBytes(str.data(), size);
				return str;
			}
			else if constexpr (is_vector_v<T>)
			{
				size_t size = Read<size_t>();
				std::vector<typename T::value_type> vec;
				vec.reserve(size);
				for (size_t i = 0; i < size; ++i)
					vec.push_back(Read<typename T::value_type>());
				return vec;
			}
			else if constexpr (is_unordered_map_v<T>)
			{
				size_t size = Read<size_t>();
				std::unordered_map<typename T::key_type, typename T::mapped_type> map;
				for (size_t i = 0; i < size; ++i)
				{
					auto key = Read<typename T::key_type>();
					auto value = Read<typename T::mapped_type>();
					map.emplace(std::move(key), std::move(value));
				}
				return map;
			}
			else if constexpr (std::is_trivially_copyable_v<T>) {
				T obj;
				ReadBytes(&obj, 1);
				return obj;
			}
			else {
				static_assert(sizeof(T) == 0, "No way to deserialize this type!");
			}
		}

		bool SaveToFile(const std::filesystem::path& path);
		bool LoadFromFile(const std::filesystem::path& path);

	private:

		size_t m_readCurrIndex = 0;
		std::vector<char> m_buffer;
	};

}


//#pragma once
//
//#include <algorithm>
//
//namespace Twisted
//{
//	class Buffer
//	{
//	public:
//
//		Buffer(size_t capacity) :
//			m_capacity(capacity),
//			m_data(new char[capacity])
//		{
//		}
//
//		Buffer(const char* data, size_t size) :
//			m_capacity(size),
//			m_data(new char[size]),
//			m_currentPosition(size)
//		{
//			std::copy(data, data + size, m_data);
//		}
//
//		Buffer(const Buffer& other) :
//			m_data(new char[other.m_capacity]),
//			m_capacity(other.m_capacity),
//			m_currentPosition(other.m_capacity)
//		{
//			std::copy(other.m_data, other.m_data + other.m_currentPosition, m_data);
//		}
//
//		Buffer& operator=(const Buffer& other)
//		{
//			if (this != &other)
//			{
//				delete[] m_data;
//				m_capacity = other.m_capacity;
//				m_currentPosition = other.m_currentPosition;
//				m_data = new char[m_capacity];
//				std::copy(other.m_data, other.m_data + other.m_currentPosition, m_data);
//			}
//			return *this;
//		}
//
//		Buffer(Buffer&& other) noexcept : //TODO: why noexcept
//			m_data(other.m_data),
//			m_capacity(other.m_capacity),
//			m_currentPosition(other.m_currentPosition)
//		{
//			other.m_data = nullptr;
//			other.m_capacity = 0;
//			other.m_currentPosition = 0;
//		}
//
//		Buffer& operator=(Buffer&& other)noexcept
//		{
//			if (this != &other)
//			{
//				delete[] m_data;
//				m_data = other.m_data;
//				m_capacity = other.m_capacity;
//				m_currentPosition = other.m_capacity;
//
//				other.m_data = nullptr;
//				other.m_capacity = 0;
//				other.m_currentPosition = 0;
//			}
//			return *this;
//		}
//
//		~Buffer()
//		{
//			delete[] m_data;
//		}
//
//		void Resize(size_t addedCapacity)
//		{
//			size_t newSize = (addedCapacity > m_capacity) ? m_capacity + addedCapacity : m_capacity * 2;
//			char* newData = new char[newSize];
//			std::copy(m_data, m_data + m_currentPosition, newData);
//			delete[] m_data;
//
//			m_data = newData;
//			m_capacity += newSize;
//		}
//		void Append(Buffer& other)
//		{
//			size_t freeCapacity = m_capacity - m_currentPosition;
//			if (freeCapacity < other.m_currentPosition)
//				Resize(other.m_currentPosition - freeCapacity);
//
//			std::copy(other.m_data, other.m_data + other.m_currentPosition, m_data + m_currentPosition);
//			m_currentPosition += other.m_currentPosition;
//		}
//
//		const char* GetData()const { return m_data; }
//		char* DataPointer() { return m_data; }
//		size_t GetDataSize()const { return m_currentPosition; }
//		size_t GetCapacity()const { return m_capacity; }
//
//	private:
//
//		char* m_data = nullptr;
//		size_t m_capacity = 0;
//		size_t m_currentPosition = 0;
//	};
//}

//bool WriteBufferToFile(const std::filesystem::path& filepath, const Buffer& buffer)
//{
//	std::error_code ec;
//	std::ofstream file(filepath, std::ios::out | std::ios::binary);
//	if (!file)
//	{
//		TWISTED_WARN("Failed to open file for writing: ");
//		return false;
//	}
//
//	size_t dataSize = buffer.GetDataSize();
//	file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
//	file.write(buffer.GetData(), dataSize);
//	file.close();
//
//	if (ec || !file.good())
//	{
//		TWISTED_WARN("Error occurred while writing to file: ");
//		return false;
//	}
//
//	return true;
//}
//
//Buffer ReadBufferFromFile(const std::filesystem::path& filepath)
//{
//	std::ifstream file(filepath, std::ios::in | std::ios::binary);
//	if (!file)
//	{
//		TWISTED_WARN("Failed to open file for reading: ");
//		return Buffer(0);
//	}
//
//	size_t dataSize = 0;
//	file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
//
//	Buffer buffer(dataSize);
//	file.read(buffer.DataPointer(), dataSize);
//	file.close();
//
//	if (!file.good())
//	{
//		TWISTED_WARN("Error occurred while reading from file: ");
//		return Buffer(0);
//	}
//
//	return buffer;
//}
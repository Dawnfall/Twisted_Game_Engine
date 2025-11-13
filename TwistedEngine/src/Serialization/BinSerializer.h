#pragma once

//TODO: byte order normalization... little/big endian

#include "AppCore.h"
#include "SerializedBuffer.h"

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
	T readFromBuffer(BinSerializer& buffer,void* data) = delete;

	// Concepts for custom free functions
	template<typename T>
	concept HasWriteToBuffer = requires(const T & obj, BinSerializer & buf, void* data) {
		writeToBuffer(obj, buf,data);
	};

	template<typename T>
	concept HasReadFromBuffer = requires(BinSerializer & buf, void* data) {
		{ readFromBuffer<T>(buf, data) } -> std::same_as<T>;
	};

	class TWISTED_API BinSerializer:public SerializedBuffer
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
		size_t GetCurrentReadIndex()const { return m_readCurrIndex; }
		void SkipReadIndex(size_t step) { m_readCurrIndex += step; }

		void IncreaseIndex(size_t delta) { m_readCurrIndex += delta; }
		std::vector<char>& GetData() { return m_buffer; }

		template<typename T>
		void WriteBytes(const T* obj, size_t count, void* data)
		{
			static_assert(std::is_trivially_copyable_v<T>, "WriteBytes requires trivially copyable types");

			size_t sizeInBytes = count * sizeof(T);

			size_t oldSize = m_buffer.size();
			EnsureCapacity(oldSize + sizeInBytes);  // ensure capacity, not size

			m_buffer.resize(oldSize + sizeInBytes); // actually increase size
			std::memcpy(m_buffer.data() + oldSize, obj, sizeInBytes);
		}

		template<typename T>
		void ReadBytes(T* obj, size_t count, void* data)
		{
			static_assert(std::is_trivially_copyable_v<T>, "ReadBytes requires trivially copyable types");

			size_t sizeInBytes = count * sizeof(T);
			if (m_readCurrIndex + sizeInBytes > m_buffer.size())
			{
				throw std::runtime_error("BinSerializer: Read out of bounds");
			}
			std::memcpy(obj, m_buffer.data() + m_readCurrIndex, sizeInBytes);
			m_readCurrIndex += sizeInBytes;
		}

		template<typename T>
		void Write(const T& obj,void* data) {
			if constexpr (HasWriteToBuffer<T>) {
				writeToBuffer(obj, *this,data);  // custom free function
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				Write<size_t>(obj.size(), data);
				EnsureCapacity(obj.size());
				m_buffer.insert(m_buffer.end(), obj.begin(), obj.end());
			}
			else if constexpr (is_vector_v<T>)
			{
				Write<size_t>(obj.size(), data);
				for (const auto& element : obj)
					Write(element,data); // recursively write elements
			}
			else if constexpr (is_unordered_map_v<T>)
			{
				Write<size_t>(obj.size(),data);
				for (const auto& [key, value] : obj)
				{
					Write(key, data);
					Write(value, data);
				}
			}
			else if constexpr (std::is_trivially_copyable_v<T>) {
				WriteBytes(&obj, 1, data);        // fallback for trivially copyable types (includes enums)
			}
			else {
				static_assert(sizeof(T) == 0, "No way to serialize this type!");
			}
		}

		template<typename T>
		void PatchBytes(const T& obj,size_t position)
		{
			std::memcpy(m_buffer.data() + position, &obj, sizeof(T));
		}

		template<typename T>
		T Read(void* data) {
			if constexpr (HasReadFromBuffer<T>)
			{
				return readFromBuffer<T>(*this,data);
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				size_t size = Read<size_t>(data);
				std::string str(size, '\0');
				ReadBytes(str.data(), size,data);
				return str;
			}
			else if constexpr (is_vector_v<T>)
			{
				size_t size = Read<size_t>(data);
				std::vector<typename T::value_type> vec;
				vec.reserve(size);
				for (size_t i = 0; i < size; ++i)
					vec.push_back(Read<typename T::value_type>(data));
				return vec;
			}
			else if constexpr (is_unordered_map_v<T>)
			{
				size_t size = Read<size_t>(data);
				std::unordered_map<typename T::key_type, typename T::mapped_type> map;
				for (size_t i = 0; i < size; ++i)
				{
					auto key = Read<typename T::key_type>(data);
					auto value = Read<typename T::mapped_type>(data);
					map.emplace(std::move(key), std::move(value));
				}
				return map;
			}
			else if constexpr (std::is_trivially_copyable_v<T>) {
				T obj;
				ReadBytes(&obj, 1,data);
				return obj;
			}
			else {
				static_assert(sizeof(T) == 0, "No way to deserialize this type!");
			}
		}

		bool SaveToFile(const std::filesystem::path& path)override;
		bool LoadFromFile(const std::filesystem::path& path)override;

	private:

		size_t m_readCurrIndex = 0;
		std::vector<char> m_buffer;
	};

}



#pragma once
#include "twistedpch.h"

namespace Twisted
{
	class SerializationBuffer
	{
	public:
		template <typename T>
		void Write(const T& data)
		{
			size_t emptySize = m_buffer.capacity() - m_buffer.size();
			size_t writeSize = sizeof(T);

			if (emptySize < writeSize)
				Resize((m_buffer.size() + writeSize) * 2);

			const char* dataPtr = reinterpret_cast<const char*>(&data);
			m_buffer.insert(m_buffer.end(), dataPtr, dataPtr + writeSize);
		}

		template <typename T>
		void WriteVec(const std::vector<T>& vec)
		{
			size_t emptySize = m_buffer.capacity() - m_buffer.size();
			size_t vecByteSize = vec.size() * sizeof(T);
			size_t writeSize = vecByteSize + sizeof(size_t);

			if (emptySize < writeSize)
				Resize((m_buffer.size() + writeSize) * 2);

			Write(vec.size());
			m_buffer.insert(m_buffer.end(), reinterpret_cast<const char*>(vec.data()), reinterpret_cast<const char*>(vec.data()) + vecByteSize);
		}

		void Resize(size_t newSize)
		{
			m_buffer.reserve(newSize);
		}

		template<typename T>
		T Read()
		{
			char buffer[sizeof(T)];
			std::memcpy(buffer, m_buffer.data() + m_readCurrIndex, sizeof(T));
			m_readCurrIndex += sizeof(T);
			return *reinterpret_cast<T*>(buffer);
		}

		template <typename T>
		std::vector<T> ReadVec()
		{
			size_t vecSize = Read<size_t>();
			std::vector<T> vec(vecSize);
			std::memcpy(vec.data(), m_buffer.data() + m_readCurrIndex, vecSize * sizeof(T));
			m_readCurrIndex += vecSize * sizeof(T);
			return vec;
		}

	private:
		size_t m_readCurrIndex = 0;
		std::vector<char> m_buffer;
	};
}
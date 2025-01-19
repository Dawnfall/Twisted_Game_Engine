#pragma once

#include "AppCore.h"

namespace Twisted
{
	class Buffer
	{
	public:

		Buffer(size_t capacity) :
			m_capacity(capacity),
			m_data(new char[capacity])
		{
		}

		Buffer(const char* data, size_t size) :
			m_capacity(size),
			m_data(new char[size]),
			m_currentPosition(size)
		{
			std::copy(data, data + size, m_data);
		}

		Buffer(const Buffer& other) :
			m_data(new char[other.m_capacity]),
			m_capacity(other.m_capacity),
			m_currentPosition(other.m_capacity)
		{
			std::copy(other.m_data, other.m_data + other.m_currentPosition, m_data);
		}

		Buffer& operator=(const Buffer& other)
		{
			if (this != &other)
			{
				delete[] m_data;
				m_capacity = other.m_capacity;
				m_currentPosition = other.m_currentPosition;
				m_data = new char[m_capacity];
				std::copy(other.m_data, other.m_data + other.m_currentPosition, m_data);
			}
			return *this;
		}

		Buffer(Buffer&& other) noexcept : //TODO: why noexcept
			m_data(other.m_data),
			m_capacity(other.m_capacity),
			m_currentPosition(other.m_currentPosition)
		{
			other.m_data = nullptr;
			other.m_capacity = 0;
			other.m_currentPosition = 0;
		}

		Buffer& operator=(Buffer&& other)noexcept
		{
			if (this != &other)
			{
				delete[] m_data;
				m_data = other.m_data;
				m_capacity = other.m_capacity;
				m_currentPosition = other.m_capacity;

				other.m_data = nullptr;
				other.m_capacity = 0;
				other.m_currentPosition = 0;
			}
			return *this;
		}

		~Buffer()
		{
			delete[] m_data;
		}

		void Resize(size_t addedCapacity)
		{
			size_t newSize = (addedCapacity > m_capacity) ? m_capacity + addedCapacity : m_capacity * 2;
			char* newData = new char[newSize];
			std::copy(m_data, m_data + m_currentPosition, newData);
			delete[] m_data;

			m_data = newData;
			m_capacity += newSize;
		}
		void Append(Buffer& other)
		{
			size_t freeCapacity = m_capacity - m_currentPosition;
			if (freeCapacity < other.m_currentPosition)
				Resize(other.m_currentPosition - freeCapacity);

			std::copy(other.m_data, other.m_data + other.m_currentPosition, m_data + m_currentPosition);
			m_currentPosition += other.m_currentPosition;
		}

		const char* GetData()const { return m_data; }
		char* DataPointer() { return m_data; }
		size_t GetDataSize()const { return m_currentPosition; }
		size_t GetCapacity()const { return m_capacity; }

	private:

		char* m_data = nullptr;
		size_t m_capacity = 0;
		size_t m_currentPosition = 0;
	};
}
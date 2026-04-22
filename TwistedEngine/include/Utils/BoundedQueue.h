#pragma once
#include <array>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <stop_token>

namespace Twisted
{
	template<typename T, size_t Capacity>
	class BoundedQueue
	{
	public:

		// Blocks if full. Returns false if stop was requested before a slot opened.
		bool Push(T value, std::stop_token stopToken)
		{
			std::unique_lock lock(m_mutex);
			m_notFull.wait(lock, stopToken, [this] { return m_count < Capacity; });
			if (m_count == Capacity)
				return false;

			m_buffer[m_tail] = std::move(value);
			m_tail = (m_tail + 1) % Capacity;
			++m_count;
			m_notEmpty.notify_one();
			return true;
		}

		// Blocks if empty. Returns nullopt if stop was requested before an item arrived.
		std::optional<T> Pop(std::stop_token stopToken)
		{
			std::unique_lock lock(m_mutex);
			m_notEmpty.wait(lock, stopToken, [this] { return m_count > 0; });
			if (m_count == 0)
				return std::nullopt;

			T val = std::move(m_buffer[m_head]);
			m_head = (m_head + 1) % Capacity;
			--m_count;
			m_notFull.notify_one();
			return val;
		}

		size_t Size() const
		{
			std::lock_guard lock(m_mutex);
			return m_count;
		}

		bool Empty() const
		{
			std::lock_guard lock(m_mutex);
			return m_count == 0;
		}

		bool Full() const
		{
			std::lock_guard lock(m_mutex);
			return m_count == Capacity;
		}

	private:
		std::array<T, Capacity> m_buffer;
		size_t m_head  = 0;
		size_t m_tail  = 0;
		size_t m_count = 0;
		mutable std::mutex m_mutex;
		std::condition_variable_any m_notFull;
		std::condition_variable_any m_notEmpty;
	};
}

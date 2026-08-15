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
		static_assert(Capacity > 0, "BoundedQueue requires a non-zero capacity.");

	public:

		// Blocks if full. Returns false only once stop was requested and no slot opened.
		bool Push(T value, std::stop_token stopToken)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (!m_notFull.wait(lock, stopToken, [this] { return m_count < Capacity; }))
				return false;

			m_buffer[m_tail] = std::move(value);
			m_tail = (m_tail + 1) % Capacity;
			++m_count;

			lock.unlock();
			m_notEmpty.notify_one();
			return true;
		}

		// Blocks if empty. Returns nullopt only once stop was requested AND the queue drained,
		// so items already waiting are still handed out after a stop request.
		std::optional<T> Pop(std::stop_token stopToken)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (!m_notEmpty.wait(lock, stopToken, [this] { return m_count > 0; }))
				return std::nullopt;

			T value = std::move(m_buffer[m_head]);
			m_buffer[m_head] = T{}; // release whatever the moved-from slot may still hold
			m_head = (m_head + 1) % Capacity;
			--m_count;

			lock.unlock();
			m_notFull.notify_one();
			return value;
		}

		size_t Size() const
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_count;
		}

		bool Empty() const
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_count == 0;
		}

		bool Full() const
		{
			std::lock_guard<std::mutex> lock(m_mutex);
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

#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <stop_token>


namespace Twisted
{
	template<typename T>
	class ThreadSafeQueue
	{
	public:

		template<typename U>
		void Push(U&& value)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				m_queue.push(std::forward<U>(value));
			}
			m_cv.notify_one();
		}

		bool TryPop(T& obj)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_queue.empty())
				return false;

			obj = std::move(m_queue.front());
			m_queue.pop();
			return true;
		}

		// Blocks until an item is available or stop is requested. Returns nullopt on stop.
		std::optional<T> WaitAndPop(std::stop_token stopToken)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, stopToken, [this] { return !m_queue.empty(); });
			if (m_queue.empty())
				return std::nullopt;

			T val = std::move(m_queue.front());
			m_queue.pop();
			return val;
		}

		bool Empty() const
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_queue.empty();
		}

	private:
		std::queue<T> m_queue;
		mutable std::mutex m_mutex;
		std::condition_variable_any m_cv;
	};


}

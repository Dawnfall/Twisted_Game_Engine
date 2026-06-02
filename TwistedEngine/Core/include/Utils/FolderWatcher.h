#pragma once
#include <filesystem>
#include <thread>
#include <vector>
#include "AppCore.h"
#include "Utils/ThreadSafeQueue.h"

namespace Twisted
{
	class TWISTED_API FolderWatcher
	{
	public:
		FolderWatcher() = default;
		~FolderWatcher();

		void Watch(const std::filesystem::path& folder);
		void Stop();

		// Drains all pending paths since last call. Call from main thread only.
		std::vector<std::filesystem::path> Poll();

	private:
		std::jthread m_thread;
		ThreadSafeQueue<std::filesystem::path> m_queue;
	};
}

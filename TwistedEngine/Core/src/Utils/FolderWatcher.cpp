#include "Utils/FolderWatcher.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Twisted
{
	FolderWatcher::~FolderWatcher()
	{
		Stop();
	}

	void FolderWatcher::Watch(const std::filesystem::path& folder)
	{
		Stop();

#ifdef _WIN32
		m_thread = std::jthread([this, folder](const std::stop_token& token)
		{
			HANDLE dir = CreateFileW(
				folder.wstring().c_str(),
				FILE_LIST_DIRECTORY,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr
			);
			if (dir == INVALID_HANDLE_VALUE)
				return;

			HANDLE cancelEvt = CreateEvent(nullptr, TRUE, FALSE, nullptr);
			std::stop_callback stopCb(token, [cancelEvt] { SetEvent(cancelEvt); });

			OVERLAPPED ov{};
			ov.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

			alignas(DWORD) char buf[4096];
			HANDLE waitHandles[] = { ov.hEvent, cancelEvt };

			while (!token.stop_requested())
			{
				ResetEvent(ov.hEvent);
				BOOL ok = ReadDirectoryChangesW(
					dir, buf, sizeof(buf), TRUE,
					FILE_NOTIFY_CHANGE_FILE_NAME,
					nullptr, &ov, nullptr
				);
				if (!ok)
					break;

				DWORD wait = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);
				if (wait != WAIT_OBJECT_0)
					break;

				DWORD bytes = 0;
				if (!GetOverlappedResult(dir, &ov, &bytes, FALSE) || bytes == 0)
					continue;

				auto* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buf);
				while (true)
				{
					if (info->Action == FILE_ACTION_ADDED || info->Action == FILE_ACTION_RENAMED_NEW_NAME)
					{
						std::wstring name(info->FileName, info->FileNameLength / sizeof(WCHAR));
						m_queue.Push(folder / name);
					}
					if (info->NextEntryOffset == 0)
						break;
					info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
						reinterpret_cast<char*>(info) + info->NextEntryOffset);
				}
			}

			CancelIo(dir);
			// Wait for the pending overlapped IO to complete/cancel before closing handles.
			DWORD unused = 0;
			GetOverlappedResult(dir, &ov, &unused, TRUE);
			CloseHandle(ov.hEvent);
			CloseHandle(cancelEvt);
			CloseHandle(dir);
		});
#endif
	}

	void FolderWatcher::Stop()
	{
		if (m_thread.joinable())
		{
			m_thread.request_stop();
			m_thread.join();
		}
	}

	std::vector<std::filesystem::path> FolderWatcher::Poll()
	{
		std::vector<std::filesystem::path> result;
		std::filesystem::path p;
		while (m_queue.TryPop(p))
			result.push_back(std::move(p));
		return result;
	}
}

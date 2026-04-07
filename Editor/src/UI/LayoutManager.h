#pragma once

#include <filesystem>

namespace Twisted::Editor
{
	class LayoutManager
	{
	public:
		static LayoutManager& GetInstance();

		void SaveLayout(const std::filesystem::path& path);
		bool LoadLayout(const std::filesystem::path& path);

		LayoutManager(const LayoutManager&) = delete;
		LayoutManager(LayoutManager&&) = delete;
		LayoutManager& operator=(const LayoutManager&) = delete;
		LayoutManager& operator=(LayoutManager&&) = delete;

	private:
		LayoutManager() = default;
	};
}

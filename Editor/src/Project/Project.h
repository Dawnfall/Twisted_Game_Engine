#pragma once
#include <filesystem>

namespace fs = std::filesystem;

namespace Twisted::Editor
{
	struct Project
	{
		Project(const std::filesystem::path& projectFolder)
		{
			RootPath = projectFolder;
		}

		std::filesystem::path GetInteralFolder() const { return RootPath / "Internal"; }
		std::filesystem::path GetMetaFolder()const { return RootPath / "Meta"; }
		std::filesystem::path GetAssetsFolder()const { return RootPath / "Assets"; }
		std::string GetName()const { return RootPath.stem().string(); }

		static std::shared_ptr<Project> CreateNewProject(const fs::path& folderPath);
		static std::shared_ptr<Project> OpenProject(const fs::path& projectFolder);
		static void ValidateProject(const fs::path& projFolder);

		std::filesystem::path RootPath;
	};
}
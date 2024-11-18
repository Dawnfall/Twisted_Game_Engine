#pragma once
#include "editorpch.h"

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

		std::filesystem::path RootPath;

		static std::shared_ptr<Project> CreateNewProject(const std::string& folderPath,const std::string& projectName);
		static std::shared_ptr<Project> OpenProject(const std::string& projectFolder);
	};
}
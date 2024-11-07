#pragma once
#include "editorpch.h"

namespace Twisted::Editor
{
	struct Project
	{
		Project(const std::string& projectFolder)
		{
			RootFolder = projectFolder;
		}




		std::string GetInteralFolder() const { return RootFolder + "/Internal"; }
		std::string GetMetaFolder()const { return RootFolder + "/Meta"; }
		std::string GetAssetsFolder()const { return RootFolder + "/Assets"; }

		std::string RootFolder = "";

		static std::shared_ptr<Project> CreateNewProject(const std::string& projectFolder);
		static std::shared_ptr<Project> OpenProject(const std::string& projectFolder);
	};
}
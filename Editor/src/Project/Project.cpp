#include "editorpch.h"
#include "Project.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"

namespace Twisted::Editor
{
	std::shared_ptr<Project> Project::CreateNewProject(const std::string& folderPath, const std::string& projectName)
	{
		std::filesystem::path projectPath = Utils::CombinePaths(folderPath, projectName);
		try
		{
			if (Utils::IsExistingFolder(projectPath))
			{
				TWISTED_WARN("Project path already exists: " + projectPath.string());
				return nullptr;
			}

			Utils::CreateFolder(projectPath);
			Utils::CreateFolder(projectPath / "Assets");
			Utils::CreateFolder(projectPath / "Meta");
			Utils::CreateFolder(projectPath / "Internal");

			return std::make_shared<Project>(projectPath);

		}
		catch (...)
		{
			TWISTED_WARN("Error creating project " + projectPath.string());
			return nullptr;
		}
	}

	std::shared_ptr<Project> Project::OpenProject(const std::string& projectFolder)
	{
		std::filesystem::path projectPath(projectFolder);

		try
		{
			if (!Utils::IsExistingFolder(projectPath))
			{
				TWISTED_WARN("Project not existing at path: " + projectPath.string());
				return nullptr;
			}
			//TODO: check subfolders

			return std::make_shared<Project>(projectPath);

		}
		catch (...)
		{
			TWISTED_WARN("Error opening project " + projectPath.string());
			return nullptr;
		}
	}



}
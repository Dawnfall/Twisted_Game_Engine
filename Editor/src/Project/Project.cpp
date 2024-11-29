#include "editorpch.h"
#include "Project.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"

namespace Twisted::Editor
{
	std::shared_ptr<Project> Project::CreateNewProject(const fs::path& folderPath)
	{
		try
		{
			if (Utils::IsExistingFolder(folderPath) && !Utils::IsEmptyDirectory(folderPath))
			{
				TWISTED_WARN("Project path must be empty directory: " + folderPath.string());
				return nullptr;
			}

			if (!Utils::IsExistingFolder(folderPath))
				Utils::CreateFolder(folderPath);

			return OpenProject(folderPath);
		}
		catch (...)
		{
			TWISTED_WARN("Error creating project " + folderPath.string());
			return nullptr;
		}
	}

	std::shared_ptr<Project> Project::OpenProject(const fs::path& projectFolder)
	{
		std::filesystem::path projectPath(projectFolder);

		try
		{
			if (!Utils::IsExistingFolder(projectPath))
			{
				TWISTED_WARN("Project not existing at path: " + projectPath.string());
				return nullptr;
			}

			Utils::CreateFolder(projectPath / "Assets");
			Utils::CreateFolder(projectPath / "Meta");
			Utils::CreateFolder(projectPath / "Internal");
			Utils::CreateNewFile(projectPath / "twisted.editor");

			return std::make_shared<Project>(projectPath);
		}
		catch (...)
		{
			TWISTED_WARN("Error opening project " + projectPath.string());
			return nullptr;
		}
	}



}
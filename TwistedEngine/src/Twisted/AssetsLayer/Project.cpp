#include "Twisted/AssetsLayer/Project.h"
#include "Debug/Logger.h"

namespace Twisted
{
	void Project::ValidateProject()const
	{
		Utils::CreateNewFile(m_rootPath / PROJECT_FILE);
		Utils::CreateFolder(GetAssetsFolder());
		Utils::CreateFolder(GetInternalFolder());
		Utils::CreateFolder(GetInternalMeshesFolder());
	}

	bool Project::SetProject(const fs::path& projectFolder)
	{
		if (!Utils::IsExisting(projectFolder))
		{
			TWISTED_WARN(std::format("projectFolder must exist {}", projectFolder.string()));
			return false;
		}
		if (Utils::IsEmptyDirectory(projectFolder) || !Utils::IsExisting(projectFolder / PROJECT_FILE))
		{
			TWISTED_WARN(std::format("Project path must be empty directory or have twisted.editor file: {}" , projectFolder.string()));
			return false;
		}

		m_rootPath = projectFolder;
		ValidateProject();

		ProjectChangeEvent.Invoke(*this);
		return true;
	}
}



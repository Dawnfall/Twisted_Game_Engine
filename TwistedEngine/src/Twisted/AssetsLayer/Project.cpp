#include "Project.h"

namespace Twisted
{
	void Project::ValidateProject()const
	{
		Utils::CreateNewFile(m_rootPath / PROJECT_FILE);
		Utils::CreateFolder(GetAssetsFolder());
		Utils::CreateFolder(GetInternalFolder());
		Utils::CreateFolder(GetInternalMeshesFolder());
	}

	void Project::SetProject(const fs::path& projectFolder)
	{
		m_rootPath = projectFolder;
		ValidateProject();

		ProjectChangeEvent.Invoke();
	}
}

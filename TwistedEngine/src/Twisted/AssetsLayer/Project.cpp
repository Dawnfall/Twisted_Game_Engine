#include "Twisted/AssetsLayer/Project.h"
#include "Twisted/AssetsLayer/ProjectConfig.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"

namespace Twisted
{
	Project::Project(fs::path rootPath) : m_rootPath(std::move(rootPath))
	{
		if (!Utils::IsExisting(m_rootPath))
		{
			TWISTED_WARN("projectFolder must exist {}", m_rootPath.string());
			return;
		}
		if (!Utils::IsEmptyDirectory(m_rootPath) && !Utils::IsExisting(m_rootPath / PROJECT_FILE))
		{
			TWISTED_WARN("Project path must be empty directory or have twisted.editor file: {}", m_rootPath.string());
			return;
		}
		m_valid = true;
		m_config.Load(m_rootPath / PROJECT_FILE);
	}
}

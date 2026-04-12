#pragma once

#include "AppCore.h"
#include "Twisted/Constants.h"
#include "Twisted/AssetsLayer/ProjectConfig.h"

#include <filesystem>
#include <string>
namespace fs = std::filesystem;

namespace Twisted
{
	class TWISTED_API Project
	{
	public:
		Project() = default;
		explicit Project(fs::path rootPath);

		bool IsValid() const { return m_valid; }

		const fs::path& GetRootPath()const { return m_rootPath; }
		fs::path GetInternalFolder() const { return m_rootPath / "Internal"; }
		fs::path GetInternalMeshesFolder()const { return m_rootPath / "Internal/Meshes"; }
		fs::path GetAssetsFolder()const { return m_rootPath / "Assets"; }
		std::string GetName()const { return m_rootPath.parent_path().filename().string(); }
		fs::path GetProjectFilePath()const { return m_rootPath / PROJECT_FILE; }

		ProjectConfig& GetConfig() { return m_config; }
		const ProjectConfig& GetConfig() const { return m_config; }

	private:
		fs::path m_rootPath;
		bool m_valid = false;
		ProjectConfig m_config;
	};
}

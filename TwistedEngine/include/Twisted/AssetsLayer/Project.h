#pragma once

#include "AppCore.h"
#include "Twisted/Constants.h"

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
		fs::path GetPanelLayoutPath()const { return m_rootPath / "EditorLayout.layout"; }
		fs::path GetProjectFilePath()const { return m_rootPath / PROJECT_FILE; }

	private:
		fs::path m_rootPath;
		bool m_valid = false;
	};
}

#pragma once

#include "AppCore.h"
#include "Utils/FileUtils.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace Twisted
{
	inline const std::filesystem::path ProjectFile = "twisted.editor";
	inline const std::filesystem::path InfoFileExt = ".info";
	inline const std::filesystem::path InternalAssetExt = ".tasset";

	class Project
	{
	public:
		Project(const Project& other) = delete;
		Project(Project&& other) = delete;
		Project& operator=(const Project& other) = delete;
		Project& operator=(Project&& other) = delete;

		static Project& GetInstance()
		{
			static Project project;
			return project;
		}

		void SetProject(const fs::path& projectFolder)
		{
			m_rootPath = projectFolder;
			ValidateProject();

			ProjectChangeEvent.Invoke();
		}

		const fs::path& GetRootPath()const { return m_rootPath; }
		fs::path GetInternalFolder() const { return m_rootPath / "Internal"; }
		fs::path GetInternalMeshesFolder()const { return m_rootPath / "Internal/Meshes"; }
		fs::path GetAssetsFolder()const { return m_rootPath / "Assets"; }
		std::string GetName()const { return m_rootPath.parent_path().filename().string(); }

		void ValidateProject() const 
		{
			Utils::CreateNewFile(m_rootPath / ProjectFile);
			Utils::CreateFolder(GetAssetsFolder());
			Utils::CreateFolder(GetInternalFolder());
			Utils::CreateFolder(GetInternalMeshesFolder());
		}

		Event<> ProjectChangeEvent;
	private:
		Project() = default;
		fs::path m_rootPath;
	};
}
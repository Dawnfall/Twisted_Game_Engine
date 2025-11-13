#pragma once

#include "AppCore.h"
#include "Utils/FileUtils.h"
#include "Utils/Event.h"
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

		bool SetProject(const fs::path& projectFolder);

		const fs::path& GetRootPath()const { return m_rootPath; }
		fs::path GetInternalFolder() const { return m_rootPath / "Internal"; }
		fs::path GetInternalMeshesFolder()const { return m_rootPath / "Internal/Meshes"; }
		fs::path GetAssetsFolder()const { return m_rootPath / "Assets"; }
		std::string GetName()const { return m_rootPath.parent_path().filename().string(); }

		void ValidateProject() const;

		Event<> ProjectChangeEvent;
	private:
		fs::path m_rootPath;
	};
}


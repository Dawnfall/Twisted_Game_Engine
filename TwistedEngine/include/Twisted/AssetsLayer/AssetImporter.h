#pragma once
#include "Debug/Logger.h"
#include "Utils/FileUtils.h"
#include "Utils/WPtr.h"
#include "Twisted/TObject.h"

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace Twisted
{
	class TWISTED_API AssetImporter
	{
	public:
		virtual std::vector<WPtrBase> Load(const fs::path& path) const = 0;
		virtual void HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const = 0;
		virtual void PostLoad([[maybe_unused]] const fs::path& path, [[maybe_unused]] std::vector<WPtrBase>& objects) const {}

		virtual bool DoAutoImport() const = 0;
		virtual std::vector<fs::path> GetAssetExtensions() const = 0;
		virtual std::string GetAssetType() const = 0;

		virtual std::string GetCreatePath() const { return ""; }
		virtual std::string DefaultFileName() const { return ""; }

		virtual void CreateNew([[maybe_unused]] const fs::path& path) const {}
		virtual bool Save([[maybe_unused]] const fs::path& path, [[maybe_unused]] const std::vector<WPtrBase>& objects) const
		{
			throw std::exception("saving not implemented for this class");
		}
	};
}

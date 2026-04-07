#pragma once
#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"

#include <filesystem>
#include <string>
#include <vector>

namespace Twisted
{
	const std::string ASSET_WORLD_TYPE = "World";

	class TWISTED_API WorldImporter : public AssetImporter
	{
	public:
		std::vector<WPtrBase> Load(const fs::path& path) const override;
		void HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const override;

		void CreateNew(const fs::path& path) const override;
		bool Save(const fs::path& path, const std::vector<WPtrBase>& objects) const override;

		std::string GetAssetType() const override { return ASSET_WORLD_TYPE; }
		bool DoAutoImport() const override { return false; }
		std::string GetCreatePath() const override { return "World"; }
		std::string DefaultFileName() const override { return "newWorld.world"; }
		std::vector<fs::path> GetAssetExtensions() const override { return { ".world" }; }
	};
}

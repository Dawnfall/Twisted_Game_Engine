#pragma once
#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"

#include <filesystem>
#include <string>
#include <vector>

namespace Twisted
{
	const std::string ASSET_MATERIAL_TYPE = "material";

	class TWISTED_API MaterialImporter : public AssetImporter
	{
	public:
		std::vector<WPtrBase> Load(const fs::path& path) const override;
		void HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const override;
		void PostLoad(const fs::path& path, std::vector<WPtrBase>& objects) const override;

		bool Save(const fs::path& path, const std::vector<WPtrBase>& objects) const override;
		void CreateNew(const fs::path& path) const override;

		std::string GetAssetType() const override { return ASSET_MATERIAL_TYPE; }
		bool DoAutoImport() const override { return true; }
		std::string GetCreatePath() const override { return "Material"; }
		std::string DefaultFileName() const override { return "newMaterial.material"; }
		std::vector<fs::path> GetAssetExtensions() const override { return { ".material" }; }
	};
}

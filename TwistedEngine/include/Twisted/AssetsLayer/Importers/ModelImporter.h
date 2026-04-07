#pragma once
#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"

#include <filesystem>
#include <string>
#include <vector>

namespace Twisted
{
	const std::string ASSET_MODEL_TYPE = "model";

	class TWISTED_API ModelImporter : public AssetImporter
	{
	public:
		std::vector<WPtrBase> Load(const fs::path& path) const override;
		void HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const override;

		std::string GetAssetType() const override { return ASSET_MODEL_TYPE; }
		bool DoAutoImport() const override { return true; }
		std::vector<fs::path> GetAssetExtensions() const override { return { ".obj" }; }
	};
}

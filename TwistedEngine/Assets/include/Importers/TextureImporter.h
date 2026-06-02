#pragma once
#include "AppCore.h"
#include "AssetImporter.h"

#include <filesystem>
#include <string>
#include <vector>

namespace Twisted
{
	const std::string ASSET_TEXTURE_TYPE = "texture";

	class TWISTED_API TextureImporter : public AssetImporter
	{
	public:
		SRef<FileImportedAsset> Load(const fs::path& path) const override;
		void HotReload(FileImportedAsset& asset) const override;

		std::string GetAssetType() const override { return ASSET_TEXTURE_TYPE; }
		bool DoAutoImport() const override { return true; }
		std::vector<fs::path> GetAssetExtensions() const override { return { ".jpg", ".jpeg", ".png", ".bmp", ".tga" }; }
	};
}

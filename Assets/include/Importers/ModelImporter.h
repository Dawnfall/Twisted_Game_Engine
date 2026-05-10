#pragma once
#include "AppCore.h"
#include "AssetImporter.h"
#include "ModelAsset.h"

#include <filesystem>
#include <string>
#include <vector>

namespace Twisted
{
	const std::string ASSET_MODEL_TYPE = "model";

	class TWISTED_API ModelImporter : public AssetImporter
	{
	public:
		SRef<FileImportedAsset> Load(const fs::path& path) const override;
		void HotReload(FileImportedAsset& asset) const override;

		std::string GetAssetType() const override { return ASSET_MODEL_TYPE; }
		bool DoAutoImport() const override { return true; }
		std::vector<fs::path> GetAssetExtensions() const override { return { ".obj", ".fbx", ".gltf", ".glb" }; }
	};
}

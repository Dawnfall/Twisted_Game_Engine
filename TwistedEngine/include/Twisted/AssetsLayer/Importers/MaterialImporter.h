#pragma once

#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "Twisted/AssetsLayer/AssetInfo.h"

#include <vector>
#include <yaml-cpp/node/node.h>
#include <filesystem>


namespace Twisted
{
	const std::string ASSET_MATERIAL_TYPE = "material";

	class TWISTED_API MaterialImporter :public AssetImporter
	{
	public:

		void ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;

		bool SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_MATERIAL_TYPE;
		}

		inline bool DoAutoImport()const override{ return true; }
		std::string GetCreatePath()const override { return "Material";}
		std::string DefaultFileName()const override { return "newMaterial.material"; }
		std::vector<fs::path> GetAssetExtensions()const override { return { ".material" }; }
		void CreateNewAsset(const fs::path& path)const override;

	};
}
//



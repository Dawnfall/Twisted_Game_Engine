#pragma once

#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "Utils/Utils.h"


namespace Twisted
{
	const std::string ASSET_MATERIAL_TYPE = "material";

	class TWISTED_API MaterialImporter :public AssetImporter
	{
	public:
		std::vector<fs::path> GetAssetExtensions()const override
		{
			return { ".material" };
		}
		inline bool DoAutoImport()const { return true; }


		ObjectsPerAsset& Import(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const override;
		void PostImport(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_MATERIAL_TYPE;
		}
	};
}
//

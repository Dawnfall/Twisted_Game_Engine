#pragma once
#include "Twisted/AssetsLayer/AssetImporter.h"

namespace Twisted
{
	const std::string ASSET_SHADER_TYPE = "shader";
	const std::string SHADER_DELIMITER = "//---//";

	class TWISTED_API ShaderImporter :public AssetImporter
	{
	public:
		std::vector<fs::path> GetAssetExtensions()const
		{
			return { ".shader" };
		}
		inline bool DoAutoImport()const { return true; }

		ObjectsPerAsset& Import(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const override;
		void PostImport(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_SHADER_TYPE;
		}
	};
}


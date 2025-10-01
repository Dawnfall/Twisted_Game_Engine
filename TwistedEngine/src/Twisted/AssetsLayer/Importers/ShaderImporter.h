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
		inline bool ImportOnStart()const { return true; }

		AssetObjects CreateObjects(AssetObjects& currObjects)const override;
		void PostCreate(const AssetInfo& assetInfo, AssetObjects& objects, AssetsLayer* assetsLayer)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_SHADER_TYPE;
		}
	};
}
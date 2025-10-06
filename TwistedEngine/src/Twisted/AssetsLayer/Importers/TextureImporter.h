#pragma once
#include "Twisted/AssetsLayer/AssetImporter.h"

namespace Twisted
{
	const std::string ASSET_TEXTURE_TYPE = "texture";

	class TWISTED_API TextureImporter : public AssetImporter
	{
	public:
		std::vector<fs::path> GetAssetExtensions()const override
		{
			return { ".jpg",".jpeg",".png",".bmp",".tga" };
		}
		inline bool ImportOnStart()const { return true; }
		

		std::vector<TObject*>& Import(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const override;
		void PostImport(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_TEXTURE_TYPE;
		}
	};
}


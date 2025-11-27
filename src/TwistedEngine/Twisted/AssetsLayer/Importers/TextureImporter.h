#pragma once
#include "Twisted/AssetsLayer/AssetImporter.h"

namespace Twisted
{
	const std::string ASSET_TEXTURE_TYPE = "texture";

	class TWISTED_API TextureImporter : public AssetImporter
	{
	public:

		void ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_TEXTURE_TYPE;
		}

		inline bool DoAutoImport()const override { return true; }
		std::vector<fs::path> GetAssetExtensions()const override { return { ".jpg",".jpeg",".png",".bmp",".tga" }; }
	};
}




#pragma once
#include "Twisted/AssetsLayer/AssetImporter.h"

namespace Twisted
{
	const std::string ASSET_WORLD_TYPE = "world";

	class TWISTED_API WorldImporter :public AssetImporter
	{
	public:
		inline std::vector<fs::path> GetAssetExtensions()const
		{
			return { ".world" };
		}
		inline bool DoAutoImport()const { return false; }

		ObjectsPerAsset& Import(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const override;
		void PostImport(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_WORLD_TYPE;
		}

		void Export()
		{
			//FillDefaultInfo();
			//SaveInfo();
			//buffer.SaveToFile(GetAssetPath());
		}

	private:

	};


}


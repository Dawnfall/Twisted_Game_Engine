#pragma once

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
		inline bool ImportOnStart()const { return true; }


		std::vector<Twisted::TObject*>& Import(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const override;
		void PostImport(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const override;

		void FillDefaultInfo(YAML::Node& node)const override
		{
			node[ASSET_TYPE_KEY] = ASSET_MATERIAL_TYPE;
		}

		//// Templated helper to fill a YAML node from a map
		//template<typename T>
		//void FillYamlFromMap(const std::string& key, const std::unordered_map<std::string, T>& inMap)
		//{
		//	YAML::Node mapNode;
		//	for (const auto& [k, v] : inMap) {
		//		mapNode[k] = v;
		//	}
		//	Node[key] = mapNode;
		//}
	};
}


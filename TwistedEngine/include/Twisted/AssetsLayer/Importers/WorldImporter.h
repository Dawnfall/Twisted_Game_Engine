#pragma once
#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "Utils/WPtrBase.h"
#include "Twisted/AssetsLayer/AssetInfo.h"

#include <string>
#include <yaml-cpp/node/node.h>
#include <vector>

namespace Twisted
{
	const std::string ASSET_WORLD_TYPE = "World";

	class TWISTED_API WorldImporter :public AssetImporter
	{
	public:
		void ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;
		void PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const override;

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

		void CreateNewAsset(const fs::path& assetPath) const override;
		bool SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects)const override;

		inline bool DoAutoImport()const override { return false; }
		std::string GetCreatePath() const override { return "World"; }
		std::string DefaultFileName() const override { return "newWorld.world"; }
		std::vector<fs::path> GetAssetExtensions()const override { return { ".world" }; }

	private:

	};


}




#pragma once
#include "Logger.h"

#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "AssetInfo.h"
#include "Twisted/RegisterLayer/ObjectID.h"
#include "Twisted/RegisterLayer/ObjectManager.h"
#include "AssetsCommon.h"

#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

namespace Twisted
{
	const std::string ASSET_TYPE_KEY = "type";

	class AssetsLayer;
	class TWISTED_API AssetImporter
	{
	public:
		virtual bool ImportOnStart()const = 0;
		virtual std::vector<fs::path> GetAssetExtensions()const = 0;
		virtual AssetObjects CreateObjects(AssetObjects& currObjects)const = 0;
		virtual void PostCreate(const AssetInfo& assetInfo, AssetObjects& objects, AssetsLayer* assetsLayer)const = 0;
		virtual void FillDefaultInfo(YAML::Node& node)const = 0;
	};
}
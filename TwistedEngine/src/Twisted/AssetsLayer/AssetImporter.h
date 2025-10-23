#pragma once
#include "Debug/Logger.h"

#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "AssetInfo.h"
#include "Utils/WPtr.h"
#include "Twisted/TObject.h"
#include "ObjectAssetEntry.h"

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;
using ObjectsPerAsset = std::unordered_map<std::string, Twisted::WPtrBase>;

namespace Twisted
{
	const std::string ASSET_TYPE_KEY = "type";

	class AssetsLayer;
	class TWISTED_API AssetImporter
	{
	public:
		virtual bool DoAutoImport()const = 0;
		virtual std::vector<fs::path> GetAssetExtensions()const = 0;
		virtual ObjectsPerAsset& Import(const AssetInfo& assetinfo, ObjectsPerAsset& objects)const = 0;
		virtual void PostImport(const AssetInfo& assetinfo, ObjectsPerAsset& objects)const = 0;
		virtual void FillDefaultInfo(YAML::Node& node)const = 0;
	};
}
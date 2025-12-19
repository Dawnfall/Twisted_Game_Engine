#pragma once
#include "Debug/Logger.h"

#include "Utils/FileUtils.h"
#include "Utils/Utils.h"

#include "AssetInfo.h"
#include "Utils/WPtr.h"
#include "Twisted/TObject.h"

#include <filesystem>
#include <yaml-cpp/yaml.h>
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
		virtual void ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const = 0;
		virtual void HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const = 0;
		virtual void PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const = 0;

		virtual bool DoAutoImport()const = 0;
		virtual std::vector<fs::path> GetAssetExtensions()const = 0;
		virtual void FillDefaultInfo(YAML::Node& node)const = 0;

		virtual std::string GetCreatePath() const { return ""; }
		virtual std::string DefaultFileName() const { return ""; }

		virtual void CreateNewAsset([[maybe_unused]] const std::filesystem::path& path) const {}
		virtual bool SaveAsset([[maybe_unused]] const fs::path& assetPath, [[maybe_unused]] const std::vector<WPtrBase>& objects)const
		{ 
			throw std::exception("saving not implemented for this class"); 
		}
	};
}


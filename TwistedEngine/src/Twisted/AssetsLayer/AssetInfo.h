#pragma once

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

#include "AssetUuid.h"
#include "Utils/YamlUtils.h"
#include "Twisted/ObjectID.h"
#include "ObjectAssetEntry.h"

#include "AppCore.h"
#include "Debug/Logger.h"
#include "Utils/FileUtils.h"
namespace fs = std::filesystem;

namespace Twisted
{
	const std::string ASSET_UUID_KEY = "uuid";
	class AssetInfo
	{
	public:

		fs::path AssetPath;
		AssetUuid Uuid;
		YAML::Node InfoNode;
		fs::file_time_type AssetLastWrite;
		fs::file_time_type InfoLastWrite;

		fs::path GetInfoPath()const { return AssetPath.string() + ".info"; }
		fs::path GetExt()const { return AssetPath.extension(); }

		bool Exists()const { return Utils::IsExisting(AssetPath); }
		bool InfoExists()const { return Utils::IsExisting(GetInfoPath()); }
		bool IsValid()const
		{
			return
				Exists() &&
				AssetLastWrite == fs::last_write_time(AssetPath) &&
				InfoLastWrite == fs::last_write_time(GetInfoPath());
		}

		bool operator!=(const AssetInfo& other) { return !((*this) == other); }

		AssetInfo(const fs::path& assetPath) :
			AssetPath(assetPath)
		{
		}


		void Validate()
		{
			AssetLastWrite = fs::last_write_time(AssetPath);
			InfoLastWrite = fs::last_write_time(GetInfoPath());
		}

		void SaveInfo()const
		{
			YamlUtils::saveNode(InfoNode,GetInfoPath());
		}
		void LoadInfo()
		{
			InfoNode = YAML::LoadFile(GetInfoPath().string());
		}

		bool operator==(const AssetInfo& other)
		{
			return
				this->AssetPath == other.AssetPath &&
				this->Uuid == other.Uuid &&
				this->AssetLastWrite == other.AssetLastWrite &&
				this->InfoLastWrite == other.InfoLastWrite;
		}
	};
}

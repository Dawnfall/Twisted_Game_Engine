#pragma once

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "AssetUuid.h"
#include "Utils/YamlUtils.h"
#include "Twisted/ObjectID.h"

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
		AssetInfo(const fs::path& assetPath) :
			m_assetPath(assetPath)
		{
		}

		const fs::path& GetAssetPath()const { return m_assetPath; }
		fs::path GetInfoPath()const { return GetAssetPath().string() + ".info"; }
		fs::path GetExt()const { return GetAssetPath().extension(); }
		const fs::file_time_type& GetAssetLastWrite() const { return m_assetLastWrite; }
		const fs::file_time_type& GetInfoLastWrite() const { return m_infoLastWrite; }
		std::string GetAssetName()const { return GetAssetPath().stem().string(); }
		bool AssetExists()const { return Utils::IsExisting(GetAssetPath()); }
		bool InfoExists()const { return Utils::IsExisting(GetInfoPath()); }

		bool IsValid()const;
		void Validate();

		bool IsBuiltIn = false;

		AssetUuid GetUuid() const { return GetInfo()[ASSET_UUID_KEY].as<AssetUuid>(AssetUuid::Invalid()); }

		bool operator==(const AssetInfo& other)
		{
			return
				this->GetAssetPath() == other.GetAssetPath() &&
				this->GetUuid() == other.GetUuid() &&
				this->GetAssetLastWrite() == other.GetAssetLastWrite() &&
				this->GetInfoLastWrite() == other.GetInfoLastWrite();
		}
		bool operator!=(const AssetInfo& other) { return !((*this) == other); }

		YAML::Node& GetInfo() { return m_infoData; }
		const YAML::Node& GetInfo()const { return m_infoData; }

		void SaveInfo()const;
		void LoadInfo();

	private:

		fs::path m_assetPath;
		YAML::Node m_infoData;
		fs::file_time_type m_assetLastWrite;
		fs::file_time_type m_infoLastWrite;
	};
}

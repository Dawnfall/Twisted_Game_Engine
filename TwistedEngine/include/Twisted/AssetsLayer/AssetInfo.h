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
	class AssetImporter;
	const std::string ASSET_UUID_KEY = "uuid";
	const std::string ASSET_TYPE_KEY = "type";

	class AssetInfo
	{
	public:
		virtual std::string GetAssetName()const=0;
		virtual AssetUuid GetUuid() const = 0;

		bool operator==(const AssetInfo& other)
		{
			return this->GetUuid() == other.GetUuid();
		}
		bool operator!=(const AssetInfo& other) { return !((*this) == other); }
	};

	class BuiltInAssetInfo :public AssetInfo
	{
	public:
		BuiltInAssetInfo(const std::string& name, AssetUuid uuid) :
			m_name(name),
			m_uuid(uuid)
		{
		}

		std::string GetAssetName()const override { return m_name; }
		AssetUuid GetUuid() const override{ return m_uuid; }

	private:
		std::string m_name;
		AssetUuid m_uuid;
	};

	class FileAssetInfo :public AssetInfo
	{
	public:
		FileAssetInfo(const fs::path& assetPath);

		std::string GetAssetName()const override { return GetAssetPath().stem().string(); }
		AssetUuid GetUuid() const override { return GetInfo()[ASSET_UUID_KEY].as<AssetUuid>(AssetUuid::Invalid()); }

		const fs::path& GetAssetPath()const { return m_assetPath; }
		fs::path GetInfoPath()const { return GetAssetPath().string() + ".info"; }
		fs::path GetExt()const { return GetAssetPath().extension(); }
		const AssetImporter* GetImporter() const{ return m_importer; }

		void SaveInfo()const;
		void LoadInfo();

		YAML::Node& GetInfo() { return m_infoData; }
		const YAML::Node& GetInfo()const { return m_infoData; }

		bool IsValid()const;
		void Validate();

		bool AssetExists()const { return Utils::IsExisting(GetAssetPath()); }
		bool InfoExists()const { return Utils::IsExisting(GetInfoPath()); }

		const fs::file_time_type& GetAssetLastWrite() const { return m_assetLastWrite; }
		const fs::file_time_type& GetInfoLastWrite() const { return m_infoLastWrite; }

	private:
		AssetImporter* m_importer = nullptr;
		fs::path m_assetPath="";
		YAML::Node m_infoData;
		fs::file_time_type m_infoLastWrite;
		fs::file_time_type m_assetLastWrite;
	};
}



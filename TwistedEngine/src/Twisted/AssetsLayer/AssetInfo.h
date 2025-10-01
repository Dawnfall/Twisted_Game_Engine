#pragma once

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

#include "AssetUuid.h"
#include "Twisted/RegisterLayer/ObjectID.h"
#include "AssetsCommon.h"

#include "AppCore.h"
#include "Logger.h"

namespace fs = std::filesystem;

namespace Twisted
{
	const std::string ASSET_UUID_KEY = "uuid";
	class AssetInfo
	{
	public:

		fs::path AssetPath;
		YAML::Node InfoNode;
		fs::file_time_type AssetLastWrite;
		fs::file_time_type InfoLastWrite;

		fs::path GetInfoPath()const { return AssetPath.string() + ".info"; }
		AssetUuid GetUuid() const { return InfoNode[ASSET_UUID_KEY].as<AssetUuid>(); }
		fs::path GetExt()const { return AssetPath.extension(); }

		bool Exists()const { return Utils::IsExisting(AssetPath); }
		bool IsValid()const
		{
			return
				Exists() &&
				AssetLastWrite == fs::last_write_time(AssetPath) &&
				InfoLastWrite == fs::last_write_time(GetInfoPath());
		}

		bool operator==(const AssetInfo& other)
		{
			return
				this->AssetPath == other.AssetPath &&
				this->GetUuid() == other.GetUuid() &&
				this->AssetLastWrite == other.AssetLastWrite &&
				this->InfoLastWrite == other.InfoLastWrite;
		}
		bool operator!=(const AssetInfo& other) { return !((*this) == other); }

		static SRef<AssetInfo> CreateInfo(const fs::path& assetPath)
		{
			if (Utils::IsExisting(assetPath))
				return std::make_shared<AssetInfo>(assetPath);

			return nullptr;
		}

		AssetInfo(const fs::path& assetPath) :
			AssetPath(assetPath)
		{
			LoadInfoData();
		}

		void LoadInfoData()
		{
			fs::path infoPath = GetInfoPath();

			YAML::Node tempNode;
			if (!Utils::IsExisting(infoPath))
			{
				tempNode[ASSET_UUID_KEY] = AssetUuid::generate();
				//FillDefaultInfo(tempNode);
				SaveInfo(tempNode, infoPath);
			}
			InfoNode = YAML::LoadFile(infoPath.string());

			AssetLastWrite = fs::last_write_time(AssetPath);
			InfoLastWrite = fs::last_write_time(GetInfoPath());
		}
	private:


		void SaveInfo(const YAML::Node& node, const fs::path& assetPath)const
		{
			try
			{
				std::ofstream fout(assetPath);
				fout << node;
				fout.close();
			}
			catch (...)
			{
				TWISTED_WARN("Error writing Info file!");
			}
		}

	};
}

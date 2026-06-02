#pragma once

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>
#include <vector>

#include "AssetUuidYaml.h"
#include "Utils/YamlUtils.h"
#include "Application/ObjectID.h"
#include "AppCore.h"
#include "Debug/Logger.h"
#include "Utils/FileUtils.h"
#include "Utils/WPtr.h"
#include "Application/TObject.h"

namespace fs = std::filesystem;
namespace Twisted
{
	class AssetImporter;
	inline constexpr const char* ASSET_UUID_KEY = "uuid";
	inline constexpr const char* ASSET_TYPE_KEY = "type";

	class TWISTED_API ImportedAsset
	{
	public:
		enum class LoadState { Registered, Loaded, LoadFailed };

		ImportedAsset() = default;
		virtual ~ImportedAsset() = default;
		ImportedAsset(const ImportedAsset&) = default;
		ImportedAsset& operator=(const ImportedAsset&) = default;
		virtual std::string GetAssetName() const = 0;
		virtual AssetUuid   GetUuid()      const = 0;

		TObject* GetPrimaryObject() const
		{
			return m_objects.empty() ? nullptr : const_cast<TObject*>(m_objects[0].GetObj());
		}

		std::vector<WPtrBase>&       GetObjects()       { return m_objects; }
		const std::vector<WPtrBase>& GetObjects() const { return m_objects; }

		LoadState GetLoadState() const { return m_loadState; }
		bool      IsLoaded()     const { return m_loadState == LoadState::Loaded; }

		bool operator==(const ImportedAsset& other) const { return GetUuid() == other.GetUuid(); }
		bool operator!=(const ImportedAsset& other) const { return !(*this == other); }

	protected:
		std::vector<WPtrBase> m_objects;
		LoadState             m_loadState = LoadState::Registered;

		friend class AssetsService;
	};

	class TWISTED_API BuiltInImportedAsset : public ImportedAsset
	{
	public:
		BuiltInImportedAsset(const std::string& name, AssetUuid uuid)
			: m_name(name), m_uuid(uuid) {}

		std::string GetAssetName() const override { return m_name; }
		AssetUuid   GetUuid()      const override { return m_uuid; }

	private:
		std::string m_name;
		AssetUuid   m_uuid;
	};

	class TWISTED_API FileImportedAsset : public ImportedAsset
	{
	public:
		FileImportedAsset(const fs::path& assetPath);

		std::string GetAssetName() const override { return GetAssetPath().stem().string(); }
		AssetUuid   GetUuid()      const override { return GetInfo()[ASSET_UUID_KEY].as<AssetUuid>(AssetUuid::Invalid()); }

		const fs::path& GetAssetPath() const { return m_assetPath; }
		fs::path        GetInfoPath()  const { return m_assetPath.string() + ".info"; }
		fs::path        GetExt()       const { return m_assetPath.extension(); }

		AssetImporter*       GetImporter()       { return m_importer; }
		const AssetImporter* GetImporter() const { return m_importer; }

		void SaveInfo() const;
		void LoadInfo();

		YAML::Node&       GetInfo()       { return m_infoData; }
		const YAML::Node& GetInfo() const { return m_infoData; }

		bool IsValid() const;
		void Validate();

		bool AssetExists() const { return Utils::IsExisting(GetAssetPath()); }
		bool InfoExists()  const { return Utils::IsExisting(GetInfoPath());  }

		const fs::file_time_type& GetAssetLastWrite() const { return m_assetLastWrite; }
		const fs::file_time_type& GetInfoLastWrite()  const { return m_infoLastWrite;  }

	private:
		AssetImporter*    m_importer = nullptr;
		fs::path          m_assetPath;
		YAML::Node        m_infoData;
		fs::file_time_type m_infoLastWrite;
		fs::file_time_type m_assetLastWrite;
	};

	// Typed asset subclasses — one per importer
	class TWISTED_API TextureAsset  : public FileImportedAsset { public: using FileImportedAsset::FileImportedAsset; };
	class TWISTED_API MaterialAsset : public FileImportedAsset { public: using FileImportedAsset::FileImportedAsset; };
	class TWISTED_API ShaderAsset   : public FileImportedAsset { public: using FileImportedAsset::FileImportedAsset; };
	class TWISTED_API WorldAsset    : public FileImportedAsset { public: using FileImportedAsset::FileImportedAsset; };
	// ModelAsset is defined in ModelAsset.h (holds a ModelNode hierarchy)
}

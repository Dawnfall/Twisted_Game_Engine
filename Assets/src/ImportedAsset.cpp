#include "ImportedAsset.h"
#include "AssetImporterRegistry.h"

namespace Twisted
{
	FileImportedAsset::FileImportedAsset(const fs::path& assetPath)
		: m_assetPath(assetPath)
	{
		m_importer = AssetImporterRegistry::GetInstance().GetImporter(GetExt());
		LoadInfo();
	}

	void FileImportedAsset::LoadInfo()
	{
		if (!InfoExists())
		{
			m_infoData[ASSET_UUID_KEY] = AssetUuid::generate();

			if (!m_importer)
				return;

			m_infoData[ASSET_TYPE_KEY] = m_importer->GetAssetType();
			SaveInfo();
		}
		else
		{
			m_infoData = YAML::LoadFile(GetInfoPath().string());
		}
	}

	void FileImportedAsset::SaveInfo() const
	{
		YamlUtils::saveNode(m_infoData, GetInfoPath(), "Failed to save asset!");
	}

	void FileImportedAsset::Validate()
	{
		m_assetLastWrite = fs::last_write_time(GetAssetPath());
		m_infoLastWrite  = fs::last_write_time(GetInfoPath());
	}

	bool FileImportedAsset::IsValid() const
	{
		return
			AssetExists() &&
			GetAssetLastWrite() == fs::last_write_time(GetAssetPath()) &&
			GetInfoLastWrite()  == fs::last_write_time(GetInfoPath());
	}
}

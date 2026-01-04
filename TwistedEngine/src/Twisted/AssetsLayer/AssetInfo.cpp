#include "Twisted/AssetsLayer/AssetInfo.h"

#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

namespace Twisted
{
	void FileAssetInfo::Validate()
	{
		m_assetLastWrite = fs::last_write_time(GetAssetPath());
		m_infoLastWrite = fs::last_write_time(GetInfoPath());
	}

	bool FileAssetInfo::IsValid()const
	{
		return
			AssetExists() &&
			GetAssetLastWrite() == fs::last_write_time(GetAssetPath()) &&
			GetInfoLastWrite() == fs::last_write_time(GetInfoPath());
	}

	void FileAssetInfo::SaveInfo()const
	{
		YamlUtils::saveNode(m_infoData, GetInfoPath());
	}

	void FileAssetInfo::LoadInfo()
	{
		if (!InfoExists())
		{
			m_infoData[ASSET_UUID_KEY] = AssetUuid::generate();

			if (!GetImporter())
				return;

			GetImporter()->FillDefaultInfo(m_infoData);
			SaveInfo();
		}
		else
		{
			m_infoData = YAML::LoadFile(GetInfoPath().string());
		}

	}

	FileAssetInfo::FileAssetInfo(const fs::path& assetPath) :
		m_assetPath(assetPath)
	{
		m_importer = AssetImporterRegistry::GetInstance().GetImporter(GetExt());
		LoadInfo();
	}


}


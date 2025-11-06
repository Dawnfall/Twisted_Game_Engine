#include "AssetInfo.h"

#include "AssetImporterRegistry.h"
#include "AssetsLayer.h"

namespace Twisted
{
	void AssetInfo::Validate()
	{
		m_assetLastWrite = fs::last_write_time(GetAssetPath());
		m_infoLastWrite = fs::last_write_time(GetInfoPath());
	}

	bool AssetInfo::IsValid()const
	{
		return
			AssetExists() &&
			GetAssetLastWrite() == fs::last_write_time(GetAssetPath()) &&
			GetInfoLastWrite() == fs::last_write_time(GetInfoPath());
	}

	void AssetInfo::SaveInfo()const
	{
		YamlUtils::saveNode(m_infoData, GetInfoPath());
	}

	void AssetInfo::LoadInfo()
	{
		if (!InfoExists())
		{
			m_infoData[ASSET_UUID_KEY] = AssetUuid::generate();

			AssetImporter* importer = AssetImporterRegistry::GetInstance().GetImporter(GetExt());
			if (!importer)
				return;

			importer->FillDefaultInfo(m_infoData);
			SaveInfo();
		}
		else
		{
			m_infoData = YAML::LoadFile(GetInfoPath().string());
		}

	}


}
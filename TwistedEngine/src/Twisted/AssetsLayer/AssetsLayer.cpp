#include "AssetsLayer.h"
#include "Twisted/Application/Application.h"
#include "Utils/FileUtils.h"
#include "Logger.h"
#include <yaml-cpp/yaml.h>

//#include "Twisted/Data/Project.h"
#include "Twisted/Rendering/Mesh.h"
#include "Utils/WPtr.h"
#include "ImporterRegistry.h"
//#include <algorithm>

namespace Twisted
{
	void AssetsLayer::DeleteLoneInfos(const fs::path& assetsFolder)
	{
		static_assert(std::is_default_constructible_v<std::hash<std::filesystem::path>>,
			"std::hash<std::filesystem::path> is not available");

		std::vector<fs::path> toDelete;
		for (const auto& entry : fs::recursive_directory_iterator(assetsFolder))
			if (entry.is_regular_file())
			{
				if (entry.path().extension() == ".info" && !Utils::IsExisting(assetsFolder / entry.path().stem()))
					toDelete.emplace_back(entry.path());
			}

		for (const auto& path : toDelete)
			Utils::DeleteAtPath(path);
	}

	void AssetsLayer::RemoveDanglingAssetObjects(const fs::path& assetsFolder)
	{
		std::vector<AssetInfo*> toRemove;
		for (auto it = m_assetsByPath.begin(); it != m_assetsByPath.end(); )
			if (!it->second->Exists())
				toRemove.emplace_back(it->second.get());

		for (auto& asset : toRemove)
			RemoveAssetInfo(asset);
	}

	void AssetsLayer::RemoveAssetInfo(AssetInfo* asset)
	{
		auto it = m_assetsByPath.find(asset->AssetPath);
		if (it != m_assetsByPath.end())
		{
			auto objIt = m_assetObjects.find(asset->GetUuid());
			if (objIt != m_assetObjects.end())
			{
				for (auto obj : objIt->second)
					TObject::Destroy(obj);
				m_assetObjects.erase(objIt);
			}

			m_assetsByPath.erase(it);
			m_assetsByUuid.erase(asset->GetUuid());
		}
	}

	void AssetsLayer::ImportAssets(const fs::path& assetsFolder)
	{
		DeleteLoneInfos(assetsFolder);
		RemoveDanglingAssetObjects(assetsFolder);

		for (const auto& entry : std::filesystem::recursive_directory_iterator(assetsFolder))
		{
			ImportAsset(entry.path());
		}
	}

	void AssetsLayer::ImportAsset(const fs::path& assetPath)
	{
		auto assetInfo = GetInfo(assetPath);

		AssetImporter* importer = ImporterRegistry::GetInstance().GetImporter(assetPath.extension());
		if (!importer || !importer->ImportOnStart())
			return;

		if (!assetInfo)
		{
			auto newInfo = AssetInfo::CreateInfo(assetPath);
			if (newInfo)
			{
				assetInfo = newInfo.get();
				m_assetsByPath[newInfo->AssetPath] = newInfo;
				m_assetsByUuid[newInfo->GetUuid()] = newInfo;
				m_assetObjects[newInfo->GetUuid()] = {};

				importer->Import(*assetInfo, m_assetObjects[assetInfo->GetUuid()], this);
				importer->PostImport(*assetInfo, m_assetObjects[assetInfo->GetUuid()], this);
			}
		}
		if (!assetInfo)
			return;

		if (!assetInfo->IsValid())
		{
			assetInfo->LoadInfoData();
			importer->Import(*assetInfo, m_assetObjects[assetInfo->GetUuid()], this);
			importer->PostImport(*assetInfo, m_assetObjects[assetInfo->GetUuid()], this);
		}
	}
}
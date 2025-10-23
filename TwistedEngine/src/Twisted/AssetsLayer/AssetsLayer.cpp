#include "AssetsLayer.h"
#include "Twisted/Application/Application.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"
#include <yaml-cpp/yaml.h>

#include "Twisted/AssetsLayer/Project.h"
#include "Twisted/Rendering/Mesh.h"
#include "Utils/WPtr.h"
#include "AssetsRegistry.h"
#include "AssetImporterRegistry.h"
#include "Utils/YamlUtils.h"

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
		for (auto it = AssetsRegistry::GetInstance().m_assetsByPath.begin(); it != AssetsRegistry::GetInstance().m_assetsByPath.end(); )
			if (!it->second->Exists())
				toRemove.emplace_back(it->second.get());

		for (auto& asset : toRemove)
			RemoveAsset(asset);
	}

	void AssetsLayer::DetectAllAssets(const fs::path& assetsFolder)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(assetsFolder))
		{
			auto info = AssetsRegistry::GetInstance().GetInfo(entry.path());
			if (info)
				continue;

			AssetImporter* importer = AssetImporterRegistry::GetInstance().GetImporter(entry.path().extension());
			if (!importer)
				continue;

			info = std::make_shared<AssetInfo>(entry.path());
			
			if (!info->InfoExists())
			{
				info->Uuid = AssetUuid::generate();
				info->InfoNode[ASSET_UUID_KEY] = info->Uuid;
				importer->FillDefaultInfo(info->InfoNode);
				info->SaveInfo();
			}
			else
				info->LoadInfo();

			AssetsRegistry::GetInstance().SetInfo(info);
		}
	}

	void AssetsLayer::RemoveAsset(AssetInfo* asset)
	{
		AssetsRegistry& reg = AssetsRegistry::GetInstance();

		auto assetObjectsIt = reg.m_assetObjects.find(asset->Uuid);
		if (assetObjectsIt != reg.m_assetObjects.end())
		{
			for (auto& pair : assetObjectsIt->second)
			{
				auto objToEntryIt = reg.m_objToEntry.erase(pair.second.GetID());
				TObject::Destroy(pair.second.GetObj());
			}

			reg.m_assetObjects.erase(assetObjectsIt);
			reg.m_assetsByPath.erase(asset->AssetPath);
			reg.m_assetsByUuid.erase(asset->Uuid);
		}
	}

	void AssetsLayer::ImportAssets()
	{
		fs::path assetsFolder = Project::GetInstance().GetAssetsFolder();

		DeleteLoneInfos(assetsFolder);
		RemoveDanglingAssetObjects(assetsFolder);
		DetectAllAssets(assetsFolder);

		for (auto& [key, info] : AssetsRegistry::GetInstance().m_assetsByPath)
		{
			info->LoadInfo(); //TODO... what if info loaded but no auto import?
			ImportAsset(info.get());
		}

		for (auto& [key, info] : AssetsRegistry::GetInstance().m_assetsByPath) //TODO... maybe add to vector
		{
			PostImportAsset(info.get());
			info->Validate();
		}
	}

	void AssetsLayer::ImportAsset(AssetInfo* info)
	{
		if (!info || info->IsValid())
			return;

		AssetImporterRegistry& reg = AssetImporterRegistry::GetInstance();
		AssetsRegistry& assetsReg = AssetsRegistry::GetInstance();

		AssetImporter* importer = reg.GetImporter(info->GetExt());
		if (!importer || !importer->DoAutoImport())
			return;

		importer->Import(*info, assetsReg.m_assetObjects[info->Uuid]);
	}

	void AssetsLayer::PostImportAsset(AssetInfo* info)
	{
		if (!info || info->IsValid())
			return;

		AssetImporterRegistry& importerReg = AssetImporterRegistry::GetInstance();
		AssetsRegistry& assetsReg = AssetsRegistry::GetInstance();

		AssetImporter* importer = importerReg.GetImporter(info->GetExt());
		if (!importer || !importer->DoAutoImport())
			return;

		importer->PostImport(*info, assetsReg.m_assetObjects[info->Uuid]);
	}
}
#include "AssetsLayer.h"
#include "Twisted/Application/Application.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"
#include <yaml-cpp/yaml.h>

#include "Twisted/AssetsLayer/Project.h"
#include "Twisted/Rendering/Mesh.h"
#include "Utils/WPtr.h"
#include "AssetImporterRegistry.h"
#include "Utils/YamlUtils.h"

#include "AssetImporter.h"
namespace Twisted
{
	void AssetsLayer::ImportAsset(AssetInfo& info)
	{
		if (info.IsValid())
			return;

		AssetImporterRegistry& reg = AssetImporterRegistry::GetInstance();
		AssetsLayer& assetsReg = AssetsLayer::GetInstance();

		AssetImporter* importer = reg.GetImporter(info.GetExt());
		if (!importer || !importer->DoAutoImport())
			return;

		if (GetAssetObjects(info.GetUuid()).empty())
			importer->ImportNew(info, GetAssetObjects(info.GetUuid()));
		else
			importer->HotReload(info, GetAssetObjects(info.GetUuid()));
	}

	void AssetsLayer::PostImportAsset(AssetInfo& info)
	{
		if (info.IsValid())
			return;

		AssetImporterRegistry& importerReg = AssetImporterRegistry::GetInstance();
		AssetsLayer& assetsReg = AssetsLayer::GetInstance();

		AssetImporter* importer = importerReg.GetImporter(info.GetExt());
		if (!importer || !importer->DoAutoImport())
			return;

		importer->PostImport(info, GetAssetObjects(info.GetUuid()));

		info.Validate();
	}

	void AssetsLayer::ImportAssets()
	{
		fs::path assetsFolder = Project::GetInstance().GetAssetsFolder();

		DeleteLoneInfos(assetsFolder);
		RemoveDanglingAssetObjects(assetsFolder);
		DetectAllAssets(assetsFolder);

		for (auto& [key, info] : m_assetsByPath)
		{
			info->LoadInfo(); //TODO... what if info loaded but no auto import?
			ImportAsset(*info);
		}

		for (auto& [key, info] : m_assetsByPath) //TODO... maybe add to vector
		{
			PostImportAsset(*info);
		}
	}







	AssetUuid AssetsLayer::GetObjectAsset(const TObject* object)const
	{
		if (!object)
			return AssetUuid::Invalid();

		for (auto& [assetUuid, assetObjects] : m_assetObjects)
			for (auto& obj : assetObjects)
				if (obj.GetObj() == object)
					return assetUuid;
		return AssetUuid::Invalid();
	}

	void AssetsLayer::SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects)
	{
		auto importer = AssetImporterRegistry::GetInstance().GetImporter(assetPath.extension());
		if (!importer)
			return;

		importer->SaveAsset(assetPath, objects);
	}

	void AssetsLayer::SaveAsset(AssetUuid uuid)
	{
		if (!uuid.IsValid())
			return;

		auto info = GetInfo(uuid);
		if (!info)
			return;

		auto importer = AssetImporterRegistry::GetInstance().GetImporter(info->GetExt());
		if (!importer)
			return;

		SaveAsset(info->GetAssetPath(), GetAssetObjects(uuid));
	}

	void AssetsLayer::CreateInfo(const fs::path& assetPath)
	{
		AssetImporter* importer = AssetImporterRegistry::GetInstance().GetImporter(assetPath.extension());
		if (!importer)
			return;

		SRef<AssetInfo> newInfo = std::make_shared<AssetInfo>(assetPath);
		newInfo->LoadInfo();

		m_assetsByPath[newInfo->GetAssetPath()] = newInfo;
		m_assetsByUuid[newInfo->GetUuid()] = newInfo;
		m_assetObjects[newInfo->GetUuid()] = {};
	}

	AssetInfo* AssetsLayer::GetInfo(const fs::path& assetPath)const
	{
		auto it = m_assetsByPath.find(assetPath);
		if (it != m_assetsByPath.end())
			return it->second.get();
		return nullptr;
	}

	AssetInfo* AssetsLayer::GetInfo(const AssetUuid& uuid)const
	{
		auto it = m_assetsByUuid.find(uuid);
		if (it != m_assetsByUuid.end())
			return it->second.get();
		return nullptr;
	}

	void AssetsLayer::RemoveAsset(AssetInfo* asset)
	{
		for (auto& pair : GetAssetObjects(asset->GetUuid()))
		{
			TObject::Destroy(pair.GetObj());
		}

		m_assetsByPath.erase(asset->GetAssetPath());
		m_assetsByUuid.erase(asset->GetUuid());
		m_assetObjects.erase(asset->GetUuid());
	}

	std::vector<WPtrBase>& AssetsLayer::GetAssetObjects(AssetUuid uuid)
	{
		auto it = m_assetObjects.find(uuid);
		if (it != m_assetObjects.end())
			return it->second;
		throw std::exception("Asset not existing");
	}
	const std::vector<WPtrBase>& AssetsLayer::GetAssetObjects(AssetUuid uuid)const
	{
		auto it = m_assetObjects.find(uuid);
		if (it != m_assetObjects.end())
			return it->second;
		throw std::exception("Asset not existing");
	}
	TObject* AssetsLayer::GetAssetObject(AssetUuid uuid, const std::string& objName)
	{
		auto itAssets = m_assetObjects.find(uuid);
		if (itAssets != m_assetObjects.end())
		{
			for (auto& assetObj : itAssets->second)
				if (assetObj.GetObj()->GetName() == objName)
					return assetObj.GetObj();
		}
		return nullptr;
	}

	void AssetsLayer::AddBuiltIn(AssetUuid uuid, TObject* obj)
	{
		m_assetObjects[uuid].emplace_back(WPtrBase(obj));
	}

	void AssetsLayer::RemoveDanglingAssetObjects(const fs::path& assetsFolder)
	{
		std::vector<AssetInfo*> toRemove;
		for (auto& [path, info] : m_assetsByPath)
		{
			if (!info->AssetExists())
				toRemove.emplace_back(info.get());
		}

		for (auto& asset : toRemove)
			RemoveAsset(asset);
	}

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

	void AssetsLayer::DetectAllAssets(const fs::path& assetsFolder)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(assetsFolder))
		{
			auto info = GetInfo(entry.path());
			if (info)
				continue;

			CreateInfo(entry.path());
		}
	}
}
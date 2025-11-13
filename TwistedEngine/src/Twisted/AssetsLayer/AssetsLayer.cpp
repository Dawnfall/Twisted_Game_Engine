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
	void AssetsLayer::AutoImportAssets() //TODO:... doesnt use validation for hot reload
	{
		if (m_project.GetRootPath() == "")
			return;

		fs::path assetsFolder = m_project.GetAssetsFolder();
		DeleteLoneInfos(assetsFolder);
		RemoveDanglingAssetObjects(assetsFolder);
		DetectAllAssets(assetsFolder);

		std::vector<FileAssetInfo*> toImport;
		for (auto& [key, info] : m_assetsByPath)
		{
			//TODO:... check for validation
			toImport.emplace_back(dynamic_cast<FileAssetInfo*>(info.get()));
		}
		ImportManaged(toImport);
	}

	std::vector<WPtrBase> AssetsLayer::ImportAssetDirect(const fs::path& assetPath)const
	{
		AssetInfo* assetInfo = GetInfo(assetPath);
		FileAssetInfo* fileAssetInfo = dynamic_cast<FileAssetInfo*>(assetInfo);

		std::vector<WPtrBase> assetObjects;
		if (!fileAssetInfo || !fileAssetInfo->GetInfo())
			return assetObjects;

		fileAssetInfo->GetImporter()->ImportNew(*fileAssetInfo, assetObjects);
		fileAssetInfo->GetImporter()->PostImport(*fileAssetInfo, assetObjects);

		return assetObjects;
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

	AssetInfo* AssetsLayer::GetObjectAssetInfo(const TObject* object)const
	{
		if (!object)
			return nullptr;

		for (auto& [assetUuid, assetObjects] : m_assetObjects)
			for (auto& obj : assetObjects)
				if (obj.GetObj() == object)
					return GetInfo(assetUuid);
		return nullptr;
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

	std::vector<WPtrBase>& AssetsLayer::GetManagedAssetObjects(AssetInfo* info)
	{
		auto it = m_assetObjects.find(info->GetUuid());
		if (it != m_assetObjects.end())
			return it->second;
		static std::vector<WPtrBase> empty;
		return empty;
	}
	const std::vector<WPtrBase>& AssetsLayer::GetManagedAssetObjects(AssetInfo* info)const
	{
		auto it = m_assetObjects.find(info->GetUuid());
		if (it != m_assetObjects.end())
			return it->second;
		static std::vector<WPtrBase> empty;
		return empty;
	}

	void AssetsLayer::RemoveAsset(FileAssetInfo* asset)
	{
		for (auto& pair : GetManagedAssetObjects(asset))
		{
			TObject::Destroy(pair.GetObj());
		}

		m_assetsByPath.erase(asset->GetAssetPath());
		m_assetsByUuid.erase(asset->GetUuid());
		m_assetObjects.erase(asset->GetUuid());
	}

	void AssetsLayer::AddBuiltIn(AssetUuid uuid, TObject* obj)
	{
		SRef<BuiltInAssetInfo> builtInInfo = std::make_shared<BuiltInAssetInfo>(obj->GetName(), uuid);
		m_assetsByUuid[uuid] = builtInInfo;
		m_assetObjects[uuid].emplace_back(WPtrBase(obj));
	}

	void AssetsLayer::SaveAsset(const FileAssetInfo* info, const std::vector<WPtrBase>& objects)
	{
		if (!info->GetImporter())
			return;

		info->GetImporter()->SaveAsset(info->GetAssetPath(), objects);
		//TODO... validate
	}

	bool AssetsLayer::CreateNewAsset(const fs::path& path)
	{
		if (Utils::IsExisting(path))
		{
			TWISTED_WARN(std::format("Asset already exists at path: {}", path.string()));
			return false;
		}

		auto importer = AssetImporterRegistry::GetInstance().GetImporter(path.extension());
		importer->CreateNewAsset(path);
		AssetInfo* info = CreateInfo(path);

		if (!info || !importer->DoAutoImport())
			return true;

		std::vector<FileAssetInfo*> infos = { dynamic_cast<FileAssetInfo*>(info) };
		ImportManaged(infos);
		return true;
	}

	void AssetsLayer::SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects)
	{
		auto info = GetInfo(assetPath);

		if (!info)
		{
			if (!CreateNewAsset(assetPath))
				return;

			info = GetInfo(assetPath);
		}

		auto assetInfo = dynamic_cast<FileAssetInfo*>(info);
		SaveAsset(assetInfo, objects);
	}

	void AssetsLayer::SaveAssetManaged(FileAssetInfo* info)
	{
		if (!info || !info->GetImporter())
			return;
		SaveAsset(info, GetManagedAssetObjects(info));
	}

	AssetInfo* AssetsLayer::CreateInfo(const fs::path& assetPath)
	{
		AssetImporter* importer = AssetImporterRegistry::GetInstance().GetImporter(assetPath.extension());
		if (!importer)
			return nullptr;

		SRef<FileAssetInfo> newInfo = std::make_shared<FileAssetInfo>(assetPath);

		m_assetsByPath[newInfo->GetAssetPath()] = newInfo;
		m_assetsByUuid[newInfo->GetUuid()] = newInfo;
		if (importer->DoAutoImport())
			m_assetObjects[newInfo->GetUuid()] = {};

		return newInfo.get();
	}

	void AssetsLayer::ImportManaged(const std::vector<FileAssetInfo*>& infos)
	{
		for (auto info : infos)
		{
			info->LoadInfo(); //TODO... what if info loaded but no auto import?

			if (!info->GetImporter() || !info->GetImporter()->DoAutoImport())
				continue;

			if (GetManagedAssetObjects(info).empty())
				info->GetImporter()->ImportNew(*info, GetManagedAssetObjects(info));
			else
				info->GetImporter()->HotReload(*info, GetManagedAssetObjects(info));
		}


		for (auto info : infos) //TODO... maybe add to vector
		{
			if (!info->GetImporter() || !info->GetImporter()->DoAutoImport())
				continue;

			info->GetImporter()->PostImport(*info, GetManagedAssetObjects(info));
			info->Validate();
		}
	}

	void AssetsLayer::RemoveDanglingAssetObjects(const fs::path& assetsFolder)
	{
		std::vector<FileAssetInfo*> toRemove;
		for (auto& [path, info] : m_assetsByPath)
		{
			FileAssetInfo* assetInfo = dynamic_cast<FileAssetInfo*>(info.get());
			if (!assetInfo->AssetExists())
				toRemove.emplace_back(assetInfo);
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


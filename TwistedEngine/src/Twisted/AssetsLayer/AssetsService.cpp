#include "Twisted/AssetsLayer/AssetsService.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"

#include "Twisted/AssetsLayer/Project.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"
#include "Twisted/AssetsLayer/AssetImporter.h"

#include "AppCore.h"
#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/TObject.h"
#include "Twisted/AssetsLayer/AssetInfo.h"
#include "Utils/WPtrBase.h"

#include <yaml-cpp/node/node.h>

#include <unordered_map>
#include <list>
#include <format>
#include <type_traits>
#include <memory>
#include <string>
#include <filesystem>
#include <vector>

namespace Twisted
{
	bool AssetsService::SetProject(const fs::path& projectFolder)
	{
		Project newProject(projectFolder);
		if (!newProject.IsValid())
			return false;

		m_project = newProject;

		Utils::CreateNewFile(m_project.GetProjectFilePath());
		Utils::CreateFolder(m_project.GetAssetsFolder());
		Utils::CreateFolder(m_project.GetInternalFolder());
		Utils::CreateFolder(m_project.GetInternalMeshesFolder());

		ProjectChangeEvent.Invoke(m_project);
		return true;
	}

	void AssetsService::AutoImportAssets() //TODO:... doesnt use validation for hot reload
	{
		if (!m_project.IsValid())
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

	std::vector<WPtrBase> AssetsService::ImportAssetDirect(const fs::path& assetPath)const
	{
		AssetInfo* assetInfo = GetInfo(assetPath);
		FileAssetInfo* fileAssetInfo = dynamic_cast<FileAssetInfo*>(assetInfo);

		std::vector<WPtrBase> assetObjects;
		if (!fileAssetInfo || !fileAssetInfo->GetInfo())
			return assetObjects;

		assetObjects = fileAssetInfo->GetImporter()->Load(fileAssetInfo->GetAssetPath());
		fileAssetInfo->GetImporter()->PostLoad(fileAssetInfo->GetAssetPath(), assetObjects);

		return assetObjects;
	}

	AssetInfo* AssetsService::GetInfo(const fs::path& assetPath)const
	{
		auto it = m_assetsByPath.find(assetPath);
		if (it != m_assetsByPath.end())
			return it->second.get();
		return nullptr;
	}

	AssetInfo* AssetsService::GetInfo(const AssetUuid& uuid)const
	{
		auto it = m_assetsByUuid.find(uuid);
		if (it != m_assetsByUuid.end())
			return it->second.get();
		return nullptr;
	}

	AssetInfo* AssetsService::GetObjectAssetInfo(const TObject* object)const
	{
		if (!object)
			return nullptr;

		for (auto& [assetUuid, assetObjects] : m_assetObjects)
			for (auto& obj : assetObjects)
				if (obj.GetObj() == object)
					return GetInfo(assetUuid);
		return nullptr;
	}

	TObject* AssetsService::GetAssetObject(AssetUuid uuid, const std::string& objName)
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

	std::vector<WPtrBase>& AssetsService::GetManagedAssetObjects(AssetInfo* info)
	{
		auto it = m_assetObjects.find(info->GetUuid());
		if (it != m_assetObjects.end())
			return it->second;
		static std::vector<WPtrBase> empty;
		return empty;
	}

	const std::vector<WPtrBase>& AssetsService::GetManagedAssetObjects(AssetInfo* info)const
	{
		auto it = m_assetObjects.find(info->GetUuid());
		if (it != m_assetObjects.end())
			return it->second;
		static std::vector<WPtrBase> empty;
		return empty;
	}

	void AssetsService::RemoveAsset(FileAssetInfo* asset)
	{
		for (auto& pair : GetManagedAssetObjects(asset))
		{
			TObject::Destroy(pair.GetObj());
		}

		m_assetsByPath.erase(asset->GetAssetPath());
		m_assetsByUuid.erase(asset->GetUuid());
		m_assetObjects.erase(asset->GetUuid());
	}

	void AssetsService::AddBuiltIn(AssetUuid uuid, TObject* obj)
	{
		SRef<BuiltInAssetInfo> builtInInfo = std::make_shared<BuiltInAssetInfo>(obj->GetName(), uuid);
		m_assetsByUuid[uuid] = builtInInfo;
		m_assetObjects[uuid].emplace_back(WPtrBase(obj));
	}

	void AssetsService::SaveAsset(const FileAssetInfo* info, const std::vector<WPtrBase>& objects)
	{
		if (!info->GetImporter())
			return;

		info->GetImporter()->Save(info->GetAssetPath(), objects);
		//TODO... validate
	}

	bool AssetsService::CreateNewAsset(const fs::path& path)
	{
		if (Utils::IsExisting(path))
		{
			TWISTED_WARN(std::format("Asset already exists at path: {}", path.string()));
			return false;
		}

		auto importer = AssetImporterRegistry::GetInstance().GetImporter(path.extension());
		importer->CreateNew(path);
		AssetInfo* info = CreateInfo(path);

		if (!info || !importer->DoAutoImport())
			return true;

		std::vector<FileAssetInfo*> infos = { dynamic_cast<FileAssetInfo*>(info) };
		ImportManaged(infos);
		return true;
	}

	void AssetsService::SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects)
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

	void AssetsService::SaveAssetManaged(FileAssetInfo* info)
	{
		if (!info || !info->GetImporter())
			return;
		SaveAsset(info, GetManagedAssetObjects(info));
	}

	AssetInfo* AssetsService::CreateInfo(const fs::path& assetPath)
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

	void AssetsService::ImportManaged(const std::vector<FileAssetInfo*>& infos)
	{
		for (auto info : infos)
		{
			info->LoadInfo(); //TODO... what if info loaded but no auto import?

			if (!info->GetImporter() || !info->GetImporter()->DoAutoImport())
				continue;

			if (GetManagedAssetObjects(info).empty())
				GetManagedAssetObjects(info) = info->GetImporter()->Load(info->GetAssetPath());
			else
				info->GetImporter()->HotReload(info->GetAssetPath(), GetManagedAssetObjects(info));
		}


		for (auto info : infos) //TODO... maybe add to vector
		{
			if (!info->GetImporter() || !info->GetImporter()->DoAutoImport())
				continue;

			info->GetImporter()->PostLoad(info->GetAssetPath(), GetManagedAssetObjects(info));
			info->Validate();
		}
	}

	void AssetsService::RemoveDanglingAssetObjects(const fs::path& assetsFolder)
	{
		(void)assetsFolder;

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

	void AssetsService::DeleteLoneInfos(const fs::path& assetsFolder)
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

	void AssetsService::DetectAllAssets(const fs::path& assetsFolder)
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


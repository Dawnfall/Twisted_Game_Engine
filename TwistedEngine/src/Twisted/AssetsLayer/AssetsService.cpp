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

#include <format>
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

		Project prevProj = std::move(m_project);
		m_project = std::move(newProject);

		Utils::CreateNewFile(m_project.GetProjectFilePath());
		Utils::CreateFolder(m_project.GetAssetsFolder());
		Utils::CreateFolder(m_project.GetInternalFolder());
		Utils::CreateFolder(m_project.GetInternalMeshesFolder());

		ProjectChangeEvent.Invoke(prevProj, m_project);
		return true;
	}

	void AssetsService::AutoImportAssets()
	{
		if (!m_project.IsValid())
			return;

		fs::path assetsFolder = m_project.GetAssetsFolder();
		DeleteLoneInfos(assetsFolder);
		RemoveDanglingAssetObjects(assetsFolder);
		DetectAllAssets(assetsFolder);

		std::vector<FileAssetInfo*> toImport;
		for (auto& [uuid, managed] : m_assets)
			if (auto* fileInfo = dynamic_cast<FileAssetInfo*>(managed.info.get()))
				toImport.emplace_back(fileInfo);

		ImportManaged(toImport);
	}

	std::vector<WPtrBase> AssetsService::ImportAssetDirect(const fs::path& assetPath) const
	{
		auto* fileInfo = dynamic_cast<FileAssetInfo*>(GetInfo(assetPath));

		std::vector<WPtrBase> objects;
		if (!fileInfo || !fileInfo->GetInfo())
			return objects;

		objects = fileInfo->GetImporter()->Load(fileInfo->GetAssetPath());
		fileInfo->GetImporter()->PostLoad(fileInfo->GetAssetPath(), objects);
		return objects;
	}

	AssetUuid AssetsService::GetUuid(const fs::path& assetPath) const
	{
		auto it = m_pathIndex.find(assetPath);
		if (it != m_pathIndex.end())
			return it->second;
		return AssetUuid::Invalid();
	}

	AssetUuid AssetsService::GetObjectUuid(const TObject* object) const
	{
		if (!object)
			return AssetUuid::Invalid();
		return object->m_assetUuid;
	}

	AssetInfo* AssetsService::GetInfo(const fs::path& assetPath) const
	{
		auto it = m_pathIndex.find(assetPath);
		if (it == m_pathIndex.end())
			return nullptr;
		auto assetIt = m_assets.find(it->second);
		if (assetIt == m_assets.end())
			return nullptr;
		return assetIt->second.info.get();
	}

	AssetInfo* AssetsService::GetInfo(AssetUuid uuid) const
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			return it->second.info.get();
		return nullptr;
	}

	TObject* AssetsService::GetObject(AssetUuid uuid, const std::string& objName) const
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			for (auto& obj : it->second.objects)
				if (const TObject* raw = obj.GetObj(); raw && raw->GetName() == objName)
					return const_cast<TObject*>(raw);
		return nullptr;
	}

	std::vector<WPtrBase>& AssetsService::GetObjects(AssetUuid uuid)
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			return it->second.objects;
		static std::vector<WPtrBase> empty;
		return empty;
	}

	const std::vector<WPtrBase>& AssetsService::GetObjects(AssetUuid uuid) const
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			return it->second.objects;
		static const std::vector<WPtrBase> empty;
		return empty;
	}

	void AssetsService::RemoveAsset(FileAssetInfo* asset)
	{
		auto it = m_assets.find(asset->GetUuid());
		if (it == m_assets.end())
			return;

		for (auto& obj : it->second.objects)
			TObject::Destroy(obj.GetObj());

		m_pathIndex.erase(asset->GetAssetPath());
		m_assets.erase(it);
	}

	void AssetsService::SetUuidOnObjects(const std::vector<WPtrBase>& objects, AssetUuid uuid)
	{
		for (const auto& obj : objects)
			if (auto* tobject = const_cast<TObject*>(obj.GetObj()))
				tobject->m_assetUuid = uuid;
	}

	void AssetsService::AddBuiltIn(AssetUuid uuid, TObject* obj)
	{
		ManagedAsset managed;
		managed.info    = std::make_shared<BuiltInAssetInfo>(obj->GetName(), uuid);
		managed.objects = { WPtrBase(obj) };
		managed.state   = AssetLoadState::Loaded;
		m_assets[uuid]  = std::move(managed);
		obj->m_assetUuid = uuid;
	}

	bool AssetsService::IsLoaded(AssetUuid uuid) const
	{
		auto it = m_assets.find(uuid);
		return it != m_assets.end() && it->second.state == AssetLoadState::Loaded;
	}

	bool AssetsService::Load(AssetUuid uuid)
	{
		if (!uuid.IsValid())
			return false;

		auto it = m_assets.find(uuid);
		if (it == m_assets.end())
			return false;

		ManagedAsset& managed = it->second;
		if (managed.state == AssetLoadState::Loaded)
			return true;

		auto* info = dynamic_cast<FileAssetInfo*>(managed.info.get());
		if (!info || !info->GetImporter())
			return false;

		info->LoadInfo();
		managed.objects = info->GetImporter()->Load(info->GetAssetPath());
		info->GetImporter()->PostLoad(info->GetAssetPath(), managed.objects);
		info->Validate();

		managed.state = managed.objects.empty() ? AssetLoadState::LoadFailed : AssetLoadState::Loaded;
		if (managed.state == AssetLoadState::Loaded)
			SetUuidOnObjects(managed.objects, uuid);
		return managed.state == AssetLoadState::Loaded;
	}

	bool AssetsService::Save(AssetUuid uuid)
	{
		if (!uuid.IsValid())
			return false;

		auto it = m_assets.find(uuid);
		if (it == m_assets.end())
			return false;

		auto* info = dynamic_cast<FileAssetInfo*>(it->second.info.get());
		if (!info || !info->GetImporter())
			return false;

		return info->GetImporter()->Save(info->GetAssetPath(), it->second.objects);
	}

	bool AssetsService::CreateNewAsset(const fs::path& path)
	{
		if (Utils::IsExisting(path))
		{
			TWISTED_WARN(std::format("Asset already exists at path: {}", path.string()));
			return false;
		}

		auto* importer = AssetImporterRegistry::GetInstance().GetImporter(path.extension());
		importer->CreateNew(path);
		AssetInfo* info = CreateInfo(path);

		if (!info || !importer->DoAutoImport())
			return true;

		ImportManaged({ dynamic_cast<FileAssetInfo*>(info) });
		return true;
	}

	void AssetsService::SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects)
	{
		if (!GetInfo(assetPath))
		{
			if (!CreateNewAsset(assetPath))
				return;
		}

		auto* info = dynamic_cast<FileAssetInfo*>(GetInfo(assetPath));
		if (!info || !info->GetImporter())
			return;
		info->GetImporter()->Save(assetPath, objects);
	}

	AssetInfo* AssetsService::CreateInfo(const fs::path& assetPath)
	{
		AssetImporter* importer = AssetImporterRegistry::GetInstance().GetImporter(assetPath.extension());
		if (!importer)
			return nullptr;

		auto newInfo   = std::make_shared<FileAssetInfo>(assetPath);
		AssetUuid uuid = newInfo->GetUuid();

		ManagedAsset managed;
		managed.info  = newInfo;
		managed.state = AssetLoadState::Registered;

		m_assets[uuid]         = std::move(managed);
		m_pathIndex[assetPath] = uuid;

		return m_assets[uuid].info.get();
	}

	void AssetsService::ImportManaged(const std::vector<FileAssetInfo*>& infos)
	{
		for (auto* info : infos)
		{
			if (!info)
				continue;

			info->LoadInfo();

			if (!info->GetImporter() || !info->GetImporter()->DoAutoImport())
				continue;

			auto it = m_assets.find(info->GetUuid());
			if (it == m_assets.end())
				continue;

			ManagedAsset& managed = it->second;
			if (managed.state != AssetLoadState::Loaded)
				managed.objects = info->GetImporter()->Load(info->GetAssetPath());
			else
				info->GetImporter()->HotReload(info->GetAssetPath(), managed.objects);
		}

		for (auto* info : infos)
		{
			if (!info || !info->GetImporter() || !info->GetImporter()->DoAutoImport())
				continue;

			auto it = m_assets.find(info->GetUuid());
			if (it == m_assets.end())
				continue;

			ManagedAsset& managed = it->second;
			info->GetImporter()->PostLoad(info->GetAssetPath(), managed.objects);
			info->Validate();
			managed.state = managed.objects.empty() ? AssetLoadState::LoadFailed : AssetLoadState::Loaded;
			if (managed.state == AssetLoadState::Loaded)
				SetUuidOnObjects(managed.objects, info->GetUuid());
		}
	}

	void AssetsService::RemoveDanglingAssetObjects(const fs::path& assetsFolder)
	{
		(void)assetsFolder;

		std::vector<FileAssetInfo*> toRemove;
		for (auto& [uuid, managed] : m_assets)
			if (auto* fileInfo = dynamic_cast<FileAssetInfo*>(managed.info.get()))
				if (!fileInfo->AssetExists())
					toRemove.emplace_back(fileInfo);

		for (auto* asset : toRemove)
			RemoveAsset(asset);
	}

	void AssetsService::DeleteLoneInfos(const fs::path& assetsFolder)
	{
		static_assert(std::is_default_constructible_v<std::hash<std::filesystem::path>>,
			"std::hash<std::filesystem::path> is not available");

		std::vector<fs::path> toDelete;
		for (const auto& entry : fs::recursive_directory_iterator(assetsFolder))
			if (entry.is_regular_file())
				if (entry.path().extension() == ".info" && !Utils::IsExisting(assetsFolder / entry.path().stem()))
					toDelete.emplace_back(entry.path());

		for (const auto& path : toDelete)
			Utils::DeleteAtPath(path);
	}

	void AssetsService::DetectAllAssets(const fs::path& assetsFolder)
	{
		for (const auto& entry : fs::recursive_directory_iterator(assetsFolder))
			if (!GetInfo(entry.path()))
				CreateInfo(entry.path());
	}
}

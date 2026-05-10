#include "AssetsService.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"

#include "Application/Application.h"
#include "WindowsService.h"
#include "Project.h"
#include "AssetImporterRegistry.h"
#include "AssetImporter.h"
#include "BuiltIn/BuiltInRegistry.h"

#include "AppCore.h"
#include "AssetUuid.h"
#include "Application/TObject.h"
#include "ImportedAsset.h"
#include "Utils/WPtrBase.h"

#include <format>
#include <memory>
#include <string>
#include <filesystem>
#include <vector>

namespace Twisted
{
	void AssetsService::OnInit()
	{
	}

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

		Collections::BuiltInRegistry::GetInstance().ImportAll();
		AutoImportAssets();

		auto& config = GetProject().GetConfig();
		if (prevProj.IsValid())
			config.Save();

		m_watcher.Watch(m_project.GetAssetsFolder());
		ProjectChangeEvent.Invoke(m_project);
		return true;
	}

	void AssetsService::AutoImportAssets()
	{
		if (!m_project.IsValid())
			return;

		ScanFolder(m_project.GetAssetsFolder());
		ImportAll();
	}

	void AssetsService::ImportAll()
	{
		std::vector<AssetUuid> toImport;
		for (auto& [uuid, asset] : m_assets)
			if (dynamic_cast<FileImportedAsset*>(asset.get()))
				toImport.push_back(uuid);
		ImportManaged(toImport);
	}

	void AssetsService::ScanFolder(const fs::path& folder)
	{
		DeleteLoneInfos(folder);
		RemoveDanglingAssetObjects();
		DetectAllAssets(folder);
	}

	void AssetsService::ProcessWatchedChanges()
	{
		auto newPaths = m_watcher.Poll();
		if (newPaths.empty())
			return;

		std::vector<AssetUuid> toImport;
		for (const fs::path& path : newPaths)
		{
			if (!fs::is_regular_file(path))
				continue;
			auto* asset = RegisterAsset(path);
			if (asset && dynamic_cast<FileImportedAsset*>(asset))
				toImport.push_back(asset->GetUuid());
		}
		ImportManaged(toImport);
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
		for (auto& [uuid, asset] : m_assets)
			for (auto& obj : asset->GetObjects())
				if (obj.GetObj() == object)
					return uuid;
		return AssetUuid::Invalid();
	}

	ImportedAsset* AssetsService::GetInfo(const fs::path& assetPath) const
	{
		auto it = m_pathIndex.find(assetPath);
		if (it == m_pathIndex.end())
			return nullptr;
		auto assetIt = m_assets.find(it->second);
		if (assetIt == m_assets.end())
			return nullptr;
		return assetIt->second.get();
	}

	ImportedAsset* AssetsService::GetInfo(AssetUuid uuid) const
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			return it->second.get();
		return nullptr;
	}

	TObject* AssetsService::GetObject(AssetUuid uuid, const std::string& objName) const
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			for (auto& obj : it->second->GetObjects())
				if (const TObject* raw = obj.GetObj(); raw && raw->GetName() == objName)
					return const_cast<TObject*>(raw);
		return nullptr;
	}

	std::vector<WPtrBase>& AssetsService::GetObjects(AssetUuid uuid)
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			return it->second->GetObjects();
		static std::vector<WPtrBase> empty;
		return empty;
	}

	const std::vector<WPtrBase>& AssetsService::GetObjects(AssetUuid uuid) const
	{
		auto it = m_assets.find(uuid);
		if (it != m_assets.end())
			return it->second->GetObjects();
		static const std::vector<WPtrBase> empty;
		return empty;
	}

	void AssetsService::RemoveAsset(FileImportedAsset* asset)
	{
		auto it = m_assets.find(asset->GetUuid());
		if (it == m_assets.end())
			return;

		for (auto& obj : it->second->GetObjects())
			TObject::Destroy(obj.GetObj());

		m_pathIndex.erase(asset->GetAssetPath());
		m_assets.erase(it);
	}

	void AssetsService::AddBuiltIn(AssetUuid uuid, TObject* obj)
	{
		auto builtIn = std::make_shared<BuiltInImportedAsset>(obj->GetName(), uuid);
		builtIn->GetObjects()  = { WPtrBase(obj) };
		builtIn->m_loadState   = ImportedAsset::LoadState::Loaded;
		m_assets[uuid]         = std::move(builtIn);
	}

	bool AssetsService::IsLoaded(AssetUuid uuid) const
	{
		auto it = m_assets.find(uuid);
		return it != m_assets.end() && it->second->IsLoaded();
	}

	bool AssetsService::LoadOne(FileImportedAsset* fileAsset)
	{
		AssetUuid uuid = fileAsset->GetUuid();
		if (m_assets.find(uuid) == m_assets.end())
			return false;

		fileAsset->LoadInfo();
		// Load() may insert into m_assets (e.g. ModelImporter registering materials),
		// which can rehash and invalidate any prior iterator. Re-find after Load returns.
		auto loaded = fileAsset->GetImporter()->Load(fileAsset->GetAssetPath());

		auto it = m_assets.find(uuid);
		if (it == m_assets.end())
			return false;

		if (!loaded || loaded->GetObjects().empty())
		{
			it->second->m_loadState = ImportedAsset::LoadState::LoadFailed;
			return false;
		}

		it->second = loaded;
		it->second->m_loadState = ImportedAsset::LoadState::Loaded;
		return true;
	}

	bool AssetsService::Load(AssetUuid uuid)
	{
		if (!uuid.IsValid())
			return false;

		auto it = m_assets.find(uuid);
		if (it == m_assets.end())
			return false;

		auto& asset = it->second;
		if (asset->IsLoaded())
			return true;

		auto* fileAsset = dynamic_cast<FileImportedAsset*>(asset.get());
		if (!fileAsset || !fileAsset->GetImporter())
			return false;

		return LoadOne(fileAsset);
	}

	bool AssetsService::Save(AssetUuid uuid)
	{
		if (!uuid.IsValid())
			return false;

		auto it = m_assets.find(uuid);
		if (it == m_assets.end())
			return false;

		auto* fileAsset = dynamic_cast<FileImportedAsset*>(it->second.get());
		if (!fileAsset || !fileAsset->GetImporter())
			return false;

		return fileAsset->GetImporter()->Save(fileAsset->GetAssetPath(), fileAsset->GetObjects());
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
		ImportedAsset* asset = CreateInfo(path);

		if (!asset || !importer->DoAutoImport())
			return true;

		ImportManaged({ asset->GetUuid() });
		return true;
	}

	void AssetsService::SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects)
	{
		if (!GetInfo(assetPath))
		{
			if (!CreateNewAsset(assetPath))
				return;
		}

		auto* fileAsset = dynamic_cast<FileImportedAsset*>(GetInfo(assetPath));
		if (!fileAsset || !fileAsset->GetImporter())
			return;
		fileAsset->GetImporter()->Save(assetPath, objects);
	}

	ImportedAsset* AssetsService::RegisterAsset(const fs::path& path)
	{
		if (auto* existing = GetInfo(path))
			return existing;
		return CreateInfo(path);
	}

	void AssetsService::DeleteAsset(const fs::path& assetPath)
	{
		auto* fileAsset = dynamic_cast<FileImportedAsset*>(GetInfo(assetPath));
		if (!fileAsset)
			return;

		fs::path infoPath = fileAsset->GetInfoPath();
		RemoveAsset(fileAsset);
		Utils::DeleteAtPath(assetPath);
		Utils::DeleteAtPath(infoPath);
	}

	ImportedAsset* AssetsService::CreateInfo(const fs::path& assetPath)
	{
		if (!AssetImporterRegistry::GetInstance().GetImporter(assetPath.extension()))
			return nullptr;

		auto shell     = std::make_shared<FileImportedAsset>(assetPath);
		AssetUuid uuid = shell->GetUuid();

		m_assets[uuid]         = std::move(shell);
		m_pathIndex[assetPath] = uuid;

		return m_assets[uuid].get();
	}

	void AssetsService::ImportManaged(const std::vector<AssetUuid>& uuids)
	{
		for (const auto& uuid : uuids)
		{
			auto it = m_assets.find(uuid);
			if (it == m_assets.end())
				continue;

			auto* fileAsset = dynamic_cast<FileImportedAsset*>(it->second.get());
			if (!fileAsset || !fileAsset->GetImporter() || !fileAsset->GetImporter()->DoAutoImport())
				continue;

			if (it->second->IsLoaded())
				fileAsset->GetImporter()->HotReload(*fileAsset);
			else
				LoadOne(fileAsset);
		}
	}

	void AssetsService::RemoveDanglingAssetObjects()
	{
		std::vector<FileImportedAsset*> toRemove;
		for (auto& [uuid, asset] : m_assets)
			if (auto* fileAsset = dynamic_cast<FileImportedAsset*>(asset.get()))
				if (!fileAsset->AssetExists())
					toRemove.emplace_back(fileAsset);

		for (auto* asset : toRemove)
			RemoveAsset(asset);
	}

	void AssetsService::DeleteLoneInfos(const fs::path& assetsFolder)
	{
		std::vector<fs::path> toDelete;
		for (const auto& entry : fs::recursive_directory_iterator(assetsFolder))
			if (entry.is_regular_file())
				if (entry.path().extension() == ".info" && !Utils::IsExisting(entry.path().parent_path() / entry.path().stem()))
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

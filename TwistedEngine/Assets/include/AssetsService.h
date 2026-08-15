#pragma once
#include "Application/Service.h"

#include "AppCore.h"
#include "Utils/Event.h"
#include "AssetImporter.h"
#include "AssetImporterRegistry.h"
#include "ImportedAsset.h"

#include "Utils/WPtr.h"
#include "FolderWatcher.h"

#include <unordered_map>
#include <filesystem>

#include "Application/TObject.h"
#include "Constants.h"
#include "Project.h"


namespace Twisted
{
	class TWISTED_API AssetsService : public Service
	{
	public:
		AssetsService(Application* app, int priority) : Service(app, priority) {}

		void OnInit() override;
		void OnTerminate() override;

		AssetUuid GetUuid(const fs::path& assetPath) const;
		AssetUuid GetObjectUuid(const TObject* object) const;

		ImportedAsset* GetInfo(const fs::path& assetPath) const;
		ImportedAsset* GetInfo(AssetUuid uuid) const;

		TObject* GetObject(AssetUuid uuid, const std::string& name) const;

		template<typename T = TObject>
		T* GetObject(AssetUuid uuid) const
		{
			auto it = m_assets.find(uuid);
			if (it != m_assets.end())
				return dynamic_cast<T*>(it->second->GetPrimaryObject());
			return nullptr;
		}

		void AutoImportAssets();
		void ScanFolder(const fs::path& folder);
		void ImportAll();
		void ProcessWatchedChanges();

		template<typename T>
		std::vector<T*> GetObjectsOfType() const
		{
			std::vector<T*> objects;
			for (auto& [uuid, asset] : m_assets)
				for (auto& obj : asset->GetObjects())
					if (T* castObj = dynamic_cast<T*>(const_cast<TObject*>(obj.GetObj())))
						objects.emplace_back(castObj);
			return objects;
		}

		template<typename T = TObject>
		std::vector<T*> GetBuiltInObjectsOfType() const
		{
			std::vector<T*> objects;
			for (auto& [uuid, asset] : m_assets)
				if (dynamic_cast<BuiltInImportedAsset*>(asset.get()))
					for (auto& obj : asset->GetObjects())
						if (T* castObj = dynamic_cast<T*>(const_cast<TObject*>(obj.GetObj())))
							objects.emplace_back(castObj);
			return objects;
		}

		bool SetProject(const fs::path& projectFolder);

		Project&       GetProject()       { return m_project; }
		const Project& GetProject() const { return m_project; }

		std::vector<WPtrBase>&       GetObjects(AssetUuid uuid);
		const std::vector<WPtrBase>& GetObjects(AssetUuid uuid) const;

		void AddBuiltIn(AssetUuid uuid, TObject* obj);

		bool Load(AssetUuid uuid);
		bool IsLoaded(AssetUuid uuid) const;
		bool Save(AssetUuid uuid);

		bool CreateNewAsset(const fs::path& path);
		void SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects);
		void DeleteAsset(const fs::path& assetPath);

		// Registers an existing file as an asset without creating it.
		// Returns existing registration if already registered.
		ImportedAsset* RegisterAsset(const fs::path& path);

		Event<const Project&> ProjectChangeEvent;

	private:
		std::unordered_map<AssetUuid, SRef<ImportedAsset>> m_assets;
		std::unordered_map<fs::path, AssetUuid>            m_pathIndex;

		Project m_project;
		FolderWatcher m_watcher;

		ImportedAsset* CreateInfo(const fs::path& assetPath);
		void DetectAllAssets(const fs::path& assetsFolder);
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects();
		bool LoadOne(FileImportedAsset* fileAsset);
		void ImportManaged(const std::vector<AssetUuid>& uuids);
		void RemoveAsset(FileImportedAsset* asset);
	};
}

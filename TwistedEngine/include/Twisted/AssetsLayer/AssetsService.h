#pragma once
#include "Twisted/Application/Service.h"

#include "AppCore.h"
#include "Utils/Event.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "AssetImporterRegistry.h"
#include "AssetInfo.h"

#include "Utils/WPtr.h"

#include <unordered_map>
#include <filesystem>

#include "Twisted/TObject.h"
#include "Twisted/Constants.h"
#include "Project.h"


namespace Twisted
{
	class TWISTED_API AssetsService : public Service
	{
	public:
		AssetsService(Application* app,int priority) : Service(app, priority) {}

		AssetUuid GetUuid(const fs::path& assetPath) const;
		AssetUuid GetObjectUuid(const TObject* object) const;

		AssetInfo* GetInfo(const fs::path& assetPath) const;
		AssetInfo* GetInfo(AssetUuid uuid) const;

		TObject* GetObject(AssetUuid uuid, const std::string& name) const;

		template<typename T = TObject>
		T* GetObject(AssetUuid uuid) const
		{
			auto it = m_assets.find(uuid);
			if (it != m_assets.end() && !it->second.objects.empty())
				return dynamic_cast<T*>(const_cast<TObject*>(it->second.objects.front().GetObj()));
			return nullptr;
		}

		void AutoImportAssets();
		std::vector<WPtrBase> ImportAssetDirect(const fs::path& assetPath) const;

		template<typename T>
		std::vector<T*> GetObjectsOfType() const
		{
			std::vector<T*> objects;
			for (auto& [uuid, managed] : m_assets)
				for (auto& obj : managed.objects)
					if (T* castObj = dynamic_cast<T*>(const_cast<TObject*>(obj.GetObj())))
						objects.emplace_back(castObj);
			return objects;
		}

		bool SetProject(const fs::path& projectFolder);

		Project& GetProject() { return m_project; }
		const Project& GetProject() const { return m_project; }

		std::vector<WPtrBase>& GetObjects(AssetUuid uuid);
		const std::vector<WPtrBase>& GetObjects(AssetUuid uuid) const;

		void AddBuiltIn(AssetUuid uuid, TObject* obj);

		bool Load(AssetUuid uuid);
		bool IsLoaded(AssetUuid uuid) const;
		bool Save(AssetUuid uuid);

		bool CreateNewAsset(const fs::path& path);
		void SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects);

		Event<const Project&, const Project&> ProjectChangeEvent;

	private:
		enum class AssetLoadState { Registered, Loaded, LoadFailed };

		struct ManagedAsset
		{
			SRef<AssetInfo>       info;
			std::vector<WPtrBase> objects;
			AssetLoadState        state = AssetLoadState::Registered;
		};

		std::unordered_map<AssetUuid, ManagedAsset> m_assets;
		std::unordered_map<fs::path, AssetUuid>     m_pathIndex;

		Project m_project;

		AssetInfo* CreateInfo(const fs::path& assetPath);
		void SetUuidOnObjects(const std::vector<WPtrBase>& objects, AssetUuid uuid);
		void DetectAllAssets(const fs::path& assetsFolder);
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects(const fs::path& assetsFolder);
		void ImportManaged(const std::vector<FileAssetInfo*>& infos);
		void RemoveAsset(FileAssetInfo* asset);
	};
}

#pragma once

#include "AppCore.h"
#include "Twisted/Application/Layer.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "AssetImporterRegistry.h"
#include "AssetInfo.h"

#include "Utils/WPtr.h"

#include <unordered_map>
#include <filesystem>

#include "Twisted/TObject.h"
#include "Twisted/Constants.h"
#include "Project.h"

#include "Twisted/Rendering/Mesh.h"
#include "Twisted/BuiltIn/MeshCollections.h"

namespace fs = std::filesystem;

namespace Twisted
{
	class ObjectManager;
	class TWISTED_API AssetsLayer :public Layer
	{
	public:
		AssetsLayer(Application* app) :Layer(app) { s_instance = this; }

		inline static AssetsLayer* GetInstance()
		{
			return s_instance;
		}

		AssetInfo* GetInfo(const fs::path& assetPath)const;
		AssetInfo* GetInfo(const AssetUuid& uuid)const;
		AssetInfo* GetObjectAssetInfo(const TObject* object)const;
		TObject* GetAssetObject(AssetUuid uuid, const std::string& name);

		void AutoImportAssets();
		std::vector<WPtrBase> ImportAssetDirect(const fs::path& assetPath)const;

		template<typename T>
		std::vector<T*> GetObjectsOfType()
		{
			std::vector<T*> objects;
			for (auto& [uuid, assetObjects] : m_assetObjects)
			{
				for (auto& assetObj : assetObjects)
				{
					T* castObj = dynamic_cast<T*>(assetObj.GetObj());
					if (castObj)
						objects.emplace_back(castObj);
				}
			}
			return objects;
		}

		Project& GetProject() { return m_project; }
		const Project& GetProject()const { return m_project; }

		std::vector<WPtrBase>& GetManagedAssetObjects(AssetInfo* info);
		const std::vector<WPtrBase>& GetManagedAssetObjects(AssetInfo* info)const;

		void AddBuiltIn(AssetUuid uuid, TObject* obj);

		void SaveAsset(const FileAssetInfo* info, const std::vector<WPtrBase>& objects);

		bool CreateNewAsset(const fs::path& path);

		//for non auto importer assets
		void SaveAssetDirect(const fs::path& assetPath, const std::vector<WPtrBase>& objects);

		void SaveAssetManaged(FileAssetInfo* info);

		void LoadBuiltIn()
		{
			Mesh* triangleMesh = TObject::Create<Mesh>(Collections::triangleMeshName);
			triangleMesh->SetData(Collections::CreateTrianglePackedData(),MeshDrawType::STATIC);
			Mesh* quadMesh = TObject::Create<Mesh>(Collections::quadMeshName);
			quadMesh->SetData(Collections::CreateQuadPackedData(), MeshDrawType::STATIC);
			//Mesh* cubeMesh = TObject::Create<Mesh>(Collections::cubeMeshName, MeshParams{});
			//cubeMesh->SetData(Collections::cubeMesh);

			AddBuiltIn(Collections::triangleMeshUUID, triangleMesh);
			AddBuiltIn(Collections::quadMeshUUID, quadMesh);
			//AddBuiltIn(Collections::cubeMeshUUID, cubeMesh);
		}

	private:

		std::unordered_map<fs::path, SRef<AssetInfo>> m_assetsByPath;
		std::unordered_map<AssetUuid, SRef<AssetInfo>> m_assetsByUuid;
		std::unordered_map<AssetUuid, std::vector<WPtrBase>> m_assetObjects;

		inline static AssetsLayer* s_instance = nullptr;

	private:
		Project m_project;

		AssetInfo* CreateInfo(const fs::path& assetPath);
		void DetectAllAssets(const fs::path& assetsFolder);
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects(const fs::path& assetsFolder);
		void ImportManaged(const std::vector<FileAssetInfo*>& infos);
		void RemoveAsset(FileAssetInfo* asset);

	};
}

//namespace Twisted
//{
//	template<typename T>
//	void Serialize(const T& obj, BinSerializer& buffer) = delete;
//	template<typename T>
//	void Deserialize(T& obj, BinSerializer& buffer) = delete;
//	template<typename T>
//	void PreSerialize(const T& obj, BinSerializer& buffer) = delete;
//	template<typename T>
//	void PostDeserialize(T& obj, BinSerializer& buffer) = delete;
//}


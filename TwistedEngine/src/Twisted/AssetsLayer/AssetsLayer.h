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

namespace fs = std::filesystem;

namespace Twisted
{
	class ObjectManager;
	class TWISTED_API AssetsLayer :public Layer
	{
	public:

		AssetsLayer(Application* app) :Layer(app) { s_instance = this; }

		inline static AssetsLayer& GetInstance()
		{
			return *s_instance;
		}

	public:

		template<typename T>
		T* ImportAssetDirect(const fs::path& assetPath)
		{
			auto& importerReg = AssetImporterRegistry::GetInstance();
			auto& reg = AssetsLayer::GetInstance();

			AssetInfo* assetInfo = reg.GetInfo(assetPath);
			AssetImporter* importer = importerReg.GetImporter(assetPath.extension());

			if (!assetInfo || !importer)
				return nullptr;

			std::vector<WPtrBase> assetObjects;
			importer->ImportNew(*assetInfo, assetObjects);
			importer->PostImport(*assetInfo, assetObjects);

			if (assetObjects.empty())
				return nullptr;
			return static_cast<T*>(assetObjects.begin()->GetObj());
		}


		void ImportAssets();

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


		//for non auto importer assets
		void SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects);
		//for auto importedAssets
		void SaveAsset(AssetUuid uuid);

		AssetUuid GetObjectAsset(const TObject* object)const;

		void CreateInfo(const fs::path& assetPath);
		AssetInfo* GetInfo(const fs::path& assetPath)const;
		AssetInfo* GetInfo(const AssetUuid& uuid)const;

		void RemoveAsset(AssetInfo* asset);

		std::vector<WPtrBase>& GetAssetObjects(AssetUuid uuid);
		const std::vector<WPtrBase>& GetAssetObjects(AssetUuid uuid)const;


		TObject* GetAssetObject(AssetUuid uuid, const std::string& name);

		void AddBuiltIn(AssetUuid uuid, TObject* obj);

	private:

		void ImportAsset(AssetInfo& info);
		void PostImportAsset(AssetInfo& info);

		std::unordered_map<fs::path, SRef<AssetInfo>> m_assetsByPath;
		std::unordered_map<AssetUuid, SRef<AssetInfo>> m_assetsByUuid;
		std::unordered_map<AssetUuid, std::vector<WPtrBase>> m_assetObjects;

		inline static AssetsLayer* s_instance = nullptr;

	private:

		void DetectAllAssets(const fs::path& assetsFolder);
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects(const fs::path& assetsFolder);
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
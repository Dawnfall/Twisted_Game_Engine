#pragma once

#include "AppCore.h"
#include "Twisted/Application/Layer.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "ImporterRegistry.h"
#include "AssetInfo.h"

#include "AssetsCommon.h"
#include "Utils/WPtr.h"

#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;
namespace Twisted
{
	class ObjectManager;
	class TWISTED_API AssetsLayer :public Layer
	{
	public:
		AssetsLayer(Application* app) :Layer(app) {}

	private:

		std::unordered_map<fs::path, SRef<AssetInfo>> m_assetsByPath;
		std::unordered_map<AssetUuid, SRef<AssetInfo>> m_assetsByUuid; //not sure if needed
		//std::unordered_map<WPtr<TObject>, ObjectAssetEntry> m_objToEntry; //not sure if needed
		std::unordered_map<AssetUuid, std::vector<TObject*>> m_assetObjects;

	public:

		AssetInfo* GetInfo(const fs::path& assetPath)
		{
			auto it = m_assetsByPath.find(assetPath);
			if (it != m_assetsByPath.end())
				return it->second.get();
			return nullptr;
		}

		AssetInfo* GetInfo(const AssetUuid& uuid)
		{
			auto it = m_assetsByUuid.find(uuid);
			if (it != m_assetsByUuid.end())
				return it->second.get();
			return nullptr;
		}

		AssetInfo* LoadInfo(const fs::path& assetPath)
		{
			auto newInfo = AssetInfo::CreateInfo(assetPath);
			if (newInfo)
			{
				m_assetsByPath[assetPath] = newInfo;
				m_assetsByUuid[newInfo->GetUuid()] = newInfo;
				return newInfo.get();
			}

			return nullptr;
		}

		void ImportAssets(const fs::path& assetsFolder);



		template<typename T>
		T* ImportAssetDirect(const fs::path& assetPath)
		{
			auto assetInfo = LoadInfo(assetPath);
			AssetImporter* importer = ImporterRegistry::GetInstance().GetImporter(assetPath.extension());

			if (!assetInfo || !importer)
				return nullptr;

			std::vector<TObject*> objects;
			importer->Import(*assetInfo, objects, this);
			
			//TODO: probably remove
			//if (objects.size() != 1)
			//{
			//	TWISTED_WARN("Direct import should only create 1 object!");
			//	return nullptr;
			//}

			importer->PostImport(*assetInfo, objects, this);
			return static_cast<T*>(objects[0]);
		}

		AssetInfo* GetObjectsOriginalAsset(TObject* object);


	private:
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects(const fs::path& assetsFolder);
		void RemoveAssetInfo(AssetInfo* asset);
		void ImportAsset(const fs::path& assetPath);

	public:
		//ObjectAssetEntry GetObjectEntry(ObjectID id)
		//{
		//	auto it = m_objToEntry.find(id);
		//	if (it != m_objToEntry.end())
		//		return it->second;
		//	return ObjectAssetEntry::Invalid();
		//}
		//ObjectID GetObjectFromEntry(const ObjectAssetEntry& objEntry)
		//{
			//auto assetIt = m_assetsByUuid.find(objEntry.Uuid);
			//if (assetIt != m_assetsByUuid.end())
			//{

			//	auto objIt = assetIt->second->Objects.find(objEntry.SubID);
			//	if (objIt != assetIt->second->Objects.end())
			//		return objIt->second;
			//}
		//	return ObjectID::Invalid();
		//}
	};
}
#pragma once

#include "AppCore.h"
#include "Twisted/Application/Layer.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "AssetInfo.h"

#include "AssetsCommon.h"

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
		std::unordered_map<ObjectID, ObjectAssetEntry> m_objToEntry; //not sure if needed
		std::unordered_map<AssetUuid, AssetObjects> m_assetObjects;

		std::unordered_map<fs::path, SRef<AssetImporter>> m_extToImporter; //TODO: maybe switch to string for ext
		std::unordered_map<fs::path, SRef<AssetImporter>> m_typeToImporter; //TODO: maybe switch to string for ext


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
		void RegisterImporter()
		{
			static_assert(std::is_base_of_v<AssetImporter, T>, "T must derive from AssetImporter");

			auto importer = std::make_shared<T>();

			for (const fs::path& ext : importer->GetAssetExtensions())
				m_extToImporter[ext] = importer;
		}

		AssetImporter* GetImporter(const fs::path& extension)
		{
			auto it = m_extToImporter.find(extension);
			if (it != m_extToImporter.end())
				return it->second.get();
			return nullptr;
		}

		template<typename T>
		T* ImportAssetDirect(const fs::path& assetPath)
		{
			auto assetInfo = LoadInfo(assetPath);
			AssetImporter* importer = GetImporter(assetPath.extension());

			if (!assetInfo || !importer)
				return nullptr;

			std::vector<ObjectID> objects;
			importer->CreateObjects(objects);
			if (objects.size() != 1)
			{
				TWISTED_WARN("Direct import should only create 1 object!");
				return nullptr;
			}

			importer->PostCreate(*assetInfo, objects, this);
			return ObjectManager::GetInstance().GetIdObject<T>(objects[0]);
		}

		AssetInfo* GetObjectsOriginalAsset(ObjectID object);


	private:
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects(const fs::path& assetsFolder);
		void RemoveAssetInfo(AssetInfo* asset);
		void ImportAsset(const fs::path& assetPath);

	public:




























		ObjectAssetEntry GetObjectEntry(ObjectID id)
		{
			auto it = m_objToEntry.find(id);
			if (it != m_objToEntry.end())
				return it->second;
			return ObjectAssetEntry::Invalid();
		}
		ObjectID GetObjectFromEntry(const ObjectAssetEntry& objEntry)
		{
			//auto assetIt = m_assetsByUuid.find(objEntry.Uuid);
			//if (assetIt != m_assetsByUuid.end())
			//{

			//	auto objIt = assetIt->second->Objects.find(objEntry.SubID);
			//	if (objIt != assetIt->second->Objects.end())
			//		return objIt->second;
			//}
			return ObjectID::Invalid();
		}


	};
}
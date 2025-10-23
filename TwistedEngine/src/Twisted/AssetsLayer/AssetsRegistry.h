#pragma once

#include "AppCore.h"
#include <unordered_map>
#include <filesystem>

#include "Twisted/TObject.h"
#include "AssetInfo.h"

namespace fs = std::filesystem;
using ObjectsPerAsset = std::unordered_map<std::string, Twisted::WPtrBase>;

namespace Twisted
{
	class TWISTED_API AssetsRegistry
	{
	public:
		static AssetsRegistry& GetInstance()
		{
			static AssetsRegistry instance;
			return instance;
		}


		SRef<AssetInfo> GetInfo(const fs::path& assetPath)
		{
			auto it = m_assetsByPath.find(assetPath);
			if (it != m_assetsByPath.end())
				return it->second;
			return nullptr;
		}
		SRef<AssetInfo> GetInfo(const AssetUuid& uuid)
		{
			auto it = m_assetsByUuid.find(uuid);
			if (it != m_assetsByUuid.end())
				return it->second;
			return nullptr;
		}

		void SetInfo(SRef<AssetInfo> info)
		{
			if (!info)
				return;

			m_assetsByPath[info->AssetPath] = info;
			m_assetsByUuid[info->Uuid] = info;
			m_assetObjects[info->Uuid] = {};
		}

		ObjectAssetEntry GetObjectEntry(const TObject* object)
		{
			if (!object)
				return ObjectAssetEntry::Invalid();

			auto it = m_objToEntry.find(object->GetID());
			if (it != m_objToEntry.end())
				return it->second;
			return ObjectAssetEntry::Invalid();
		}

		template<typename T>
		T* GetObjectFromEntry(const ObjectAssetEntry& objEntry)
		{
			auto itAsset = m_assetObjects.find(objEntry.Uuid);
			if (itAsset != m_assetObjects.end())
			{
				auto itObjects = itAsset->second.find(objEntry.SubID);
				if (itObjects != itAsset->second.end())
					return dynamic_cast<T*>(itObjects->second.GetObj());
			}
			return nullptr;
		}

		const ObjectsPerAsset& GetAssetObjects(AssetUuid uuid)
		{
			auto it = m_assetObjects.find(uuid);
			if (it != m_assetObjects.end())
			{
				return it->second;
			}
			static ObjectsPerAsset empty;
			return empty;
		}

		const ObjectsPerAsset& GetAssetObjects(const fs::path& assetPath)
		{
			SRef<AssetInfo> info = GetInfo(assetPath);

			if (info)
				return GetAssetObjects(info->Uuid);
			static ObjectsPerAsset empty;
			return empty;
		}

	private:
		std::unordered_map<fs::path, SRef<AssetInfo>> m_assetsByPath;
		std::unordered_map<AssetUuid, SRef<AssetInfo>> m_assetsByUuid;
		std::unordered_map<ObjectID, ObjectAssetEntry> m_objToEntry;
		std::unordered_map<AssetUuid, ObjectsPerAsset> m_assetObjects;

	private:
		AssetsRegistry() = default;

	public:

		AssetsRegistry(const AssetsRegistry& other) = delete;
		AssetsRegistry(AssetsRegistry&& other) = delete;
		AssetsRegistry& operator=(const AssetsRegistry& other) = delete;
		AssetsRegistry& operator=(AssetsRegistry&& other) = delete;

		friend class AssetsLayer;
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
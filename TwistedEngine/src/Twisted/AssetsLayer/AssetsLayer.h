#pragma once

#include "AppCore.h"
#include "Twisted/Application/Layer.h"
#include "Twisted/AssetsLayer/AssetImporter.h"
#include "AssetImporterRegistry.h"
#include "AssetsRegistry.h"
#include "AssetInfo.h"

#include "ObjectAssetEntry.h"
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
		AssetsLayer(Application* app) :Layer(app) {}

	public:

		void ImportAssets();

		template<typename T>
		T* ImportAssetDirect(const fs::path& assetPath)
		{
			auto& importerReg = AssetImporterRegistry::GetInstance();
			auto& reg = AssetsRegistry::GetInstance();

			SRef<AssetInfo> assetInfo = reg.GetInfo(assetPath);
			AssetImporter* importer = importerReg.GetImporter(assetPath.extension());

			if (!assetInfo || !importer)
				return nullptr;

			ObjectsPerAsset objects {};

			importer->Import(*assetInfo, objects);
			importer->PostImport(*assetInfo, objects);

			return static_cast<T*>(objects[0].GetObj());
		}

		AssetInfo* GetObjectsOriginalAsset(TObject* object);



	private:

		void DetectAllAssets(const fs::path& assetsFolder);
		void DeleteLoneInfos(const fs::path& assetsFolder);
		void RemoveDanglingAssetObjects(const fs::path& assetsFolder);
		void RemoveAsset(AssetInfo* asset);

		void ImportAsset(AssetInfo* info);
		void PostImportAsset(AssetInfo* info);
	};
}
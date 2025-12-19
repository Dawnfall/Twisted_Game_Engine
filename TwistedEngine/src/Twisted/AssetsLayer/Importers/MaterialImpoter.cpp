#include "MaterialImporter.h"
//#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"
#include <filesystem>
#include <memory>
#include <yaml-cpp/node/parse.h>
#include "Twisted/AssetsLayer/AssetInfo.h"
#include "Utils/WPtr.h"
#include "Twisted/TObject.h"
#include "Utils/WPtrBase.h"
#include <yaml-cpp/node/node.h>
#include <vector>
#include "Utils/YamlUtils.h"

namespace Twisted
{
	void MaterialImporter::ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		WPtr<Material> mat(TObject::Create<Material>(assetInfo.GetAssetName()));

		objects.emplace_back(mat);
	}

	void MaterialImporter::PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		if (objects.empty())
			return;

		Material* mat = static_cast<Material*>(objects[0].GetObj());
		YAML::Node buffer = YAML::LoadFile(assetInfo.GetAssetPath().string());
		
		if (!mat)
			return;
		
		YamlDeserialize<Material>(*mat, buffer);

	}

	void MaterialImporter::HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		(void)assetInfo;
		(void)objects;
		//TODO:...
	}



	void MaterialImporter::CreateNewAsset(const fs::path& path)const
	{
		auto material = std::make_unique<Material>(path.stem().string());
		if (!material)
			return;

		YAML::Node data = YamlSerialize<Material>(*material);
		YamlUtils::saveNode(data, path);
	}

	bool MaterialImporter::SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects)const
	{
		if (objects.empty())
			return false;

		const Material* mat = static_cast<const Material*>(objects[0].GetObj());
		if (!mat)
			return false;

		YAML::Node data = YamlSerialize<Material>(*mat);
		YamlUtils::saveNode(data, assetPath);

		return true;
	}

}

REGISTER_IMPORTER(MaterialImporter)



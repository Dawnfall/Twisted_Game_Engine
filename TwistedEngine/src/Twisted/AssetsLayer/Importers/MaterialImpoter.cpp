#include "MaterialImporter.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"
#include "Twisted/ObjectManager.h"

namespace Twisted
{
	void MaterialImporter::ImportNew(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		WPtr<Material> mat(ObjectManager::Create<Material>(assetInfo.GetAssetName()));

		objects.emplace_back(mat);
	}

	void MaterialImporter::PostImport(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		if (objects.empty())
			return;

		Material* mat = static_cast<Material*>(objects[0].GetObj());
		YAML::Node buffer = YAML::LoadFile(assetInfo.GetAssetPath().string());
		mat->YAMLDeserialize(buffer);

	}

	void MaterialImporter::HotReload(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//TODO:...
	}



	void MaterialImporter::CreateNewAsset(const fs::path& path)const
	{
		auto material = std::make_unique<Material>(path.stem().string());
		YAML::Node data = material->YAMLSerialize();
		YamlUtils::saveNode(data, path);
	}

	bool MaterialImporter::SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects)const
	{
		if (objects.empty())
			return false;

		const Material* mat = static_cast<const Material*>(objects[0].GetObj());
		if (!mat)
			return false;

		YAML::Node data = mat->YAMLSerialize();
		YamlUtils::saveNode(data, assetPath);

		return true;
	}

}

REGISTER_IMPORTER(MaterialImporter)

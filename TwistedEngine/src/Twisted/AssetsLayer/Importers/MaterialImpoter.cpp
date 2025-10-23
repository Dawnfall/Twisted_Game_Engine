#include "MaterialImporter.h"
#include "Twisted/TwistedMacros.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/AssetsLayer/ObjectAssetEntry.h"

namespace Twisted
{
	ObjectsPerAsset& MaterialImporter::Import(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{
		if (objects.size() > 0)
		{
			objects[""].GetObj()->static_as<Material>()->Clear();
		}
		else
			objects[""] = WPtr<Material>(TObject::Create<Material>());

		return objects;
	}

	void MaterialImporter::PostImport(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{
		YAML::Node assetData = YAML::LoadFile(assetInfo.AssetPath.string());

		MaterialData Data;
		std::unordered_map<std::string, ObjectAssetEntry> TexIDs;

		Material* mat = static_cast<Material*>(objects[""].GetObj());

	}
}

REGISTER_IMPORTER(MaterialImporter)

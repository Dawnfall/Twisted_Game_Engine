#include "MaterialImporter.h"
#include "TwistedMacros.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/Material.h"

namespace Twisted
{
	template<typename T>
	void FillMapFromYaml(YAML::Node& node, const std::string& key, Material* mat)
	{
		if (node[key])
		{
			for (const auto& it : node[key])
			{
				mat->Set<T>(it.first.as<std::string>(), it.second.as<T>());
			}
		}
	}

	std::vector<Twisted::TObject*>& MaterialImporter::Import(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const
	{
		if (objects.size() > 0)
		{
			objects[0]->static_as<Material>()->Clear();
		}
		else
			objects.emplace_back(TObject::Create<Material>());

		return objects;
	}

	void MaterialImporter::PostImport(const AssetInfo& assetInfo, std::vector<Twisted::TObject*>& objects, AssetsLayer* assetsLayer)const
	{
		YAML::Node assetData = YAML::LoadFile(assetInfo.AssetPath.string());

		MaterialData Data;
		std::unordered_map<std::string, ObjectAssetEntry> TexIDs;

		Material* mat = static_cast<Material*>(objects[0]);

		FillMapFromYaml<bool>(assetData, "Bools", mat);
		FillMapFromYaml<int>(assetData, "ints", mat);
		FillMapFromYaml<float>(assetData, "floats", mat);
		FillMapFromYaml<Vec2f>(assetData, "vec2s", mat);
		FillMapFromYaml<Vec3f>(assetData, "vec3s", mat);
		FillMapFromYaml<Vec4f>(assetData, "vec4s", mat);
		FillMapFromYaml<Mat4x4f>(assetData, "mats4x4", mat);
		//FillMapFromYaml<ObjectAssetEntry>(assetData,"textures", mat);
	}
}

REGISTER_IMPORTER(MaterialImporter)

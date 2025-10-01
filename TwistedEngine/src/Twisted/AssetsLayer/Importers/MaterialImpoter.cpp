#include "MaterialImporter.h"

#include "Twisted/Rendering/Material.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"

namespace Twisted
{
	AssetObjects MaterialImporter::CreateObjects(AssetObjects& currObjects)const
	{
		if (currObjects.size()>0)
			currObjects[0] = ObjectManager::GetInstance().ReloadObject<Material>(currObjects[0])->getID();
		else
			currObjects.emplace_back(ObjectManager::GetInstance().CreateObject<Material>()->getID());

		return currObjects;
	}

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

	void MaterialImporter::PostCreate(const AssetInfo& assetInfo, AssetObjects& objects, AssetsLayer* assetsLayer)const
	{
		YAML::Node assetData = YAML::LoadFile(assetInfo.AssetPath.string());

		MaterialData Data;
		std::unordered_map<std::string, ObjectAssetEntry> TexIDs;

		Material* mat = ObjectManager::GetInstance().GetIdObject<Material>(objects[0]);

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
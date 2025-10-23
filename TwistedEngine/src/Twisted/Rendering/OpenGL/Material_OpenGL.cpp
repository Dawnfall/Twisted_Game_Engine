#include "Twisted/Rendering/Material.h"

#include "Twisted/AssetsLayer/ObjectAssetEntry.h"
#include "Twisted/AssetsLayer/AssetsRegistry.h"
#include "Twisted/Rendering/Shader.h"
#include <glad/glad.h>

namespace Twisted
{
	template<typename T>
	void FillMapFromYaml(const YAML::Node& node, const std::string& key, Material* mat)
	{
		if (node[key])
		{
			for (const auto& it : node[key])
			{
				mat->Set<T>(it.first.as<std::string>(), it.second.as<T>());
			}
		}
	}

	template<typename T>
	void FillYamlFromMap(YAML::Node& node, const std::unordered_map<std::string, T>& map)
	{
		for (const auto& pair : map)
		{
			node[pair.first] = pair.second;
		}
	}

	YAML::Node Material::Serialize()const
	{
		YAML::Node node;

		FillYamlFromMap<unsigned int>(node, getMap<unsigned int>());
		FillYamlFromMap<bool>(node, getMap<bool>());
		FillYamlFromMap<double>(node, getMap<double>());
		FillYamlFromMap<int>(node, getMap<int>());
		FillYamlFromMap<float>(node, getMap<float>());
		FillYamlFromMap<Vec2f>(node, getMap<Vec2f>());
		FillYamlFromMap<Vec3f>(node, getMap<Vec3f>());
		FillYamlFromMap<Vec4f>(node, getMap<Vec4f>());
		FillYamlFromMap<Vec2d>(node, getMap<Vec2d>());
		FillYamlFromMap<Vec3d>(node, getMap<Vec3d>());
		FillYamlFromMap<Vec4d>(node, getMap<Vec4d>());
		FillYamlFromMap<Vec2i>(node, getMap<Vec2i>());
		FillYamlFromMap<Vec3i>(node, getMap<Vec3i>());
		FillYamlFromMap<Vec4i>(node, getMap<Vec4i>());
		FillYamlFromMap<Mat2x2f>(node, getMap<Mat2x2f>());
		FillYamlFromMap<Mat3x3f>(node, getMap<Mat3x3f>());
		FillYamlFromMap<Mat4x4f>(node, getMap<Mat4x4f>());
		FillYamlFromMap<Mat2x2d>(node, getMap<Mat2x2d>());
		FillYamlFromMap<Mat3x3d>(node, getMap<Mat3x3d>());
		FillYamlFromMap<Mat4x4d>(node, getMap<Mat4x4d>());

		for (const auto& pair : getMap<WPtr<Texture>>())
		{
			node[pair.first] = AssetsRegistry::GetInstance().GetObjectEntry(pair.second.get());
		}

		return node;
	}

	void Material::Deserialize(const YAML::Node& node)
	{
		FillMapFromYaml<unsigned int>(node, "UnsignedInts", this);
		FillMapFromYaml<bool>(node, "Bools", this);
		FillMapFromYaml<double>(node, "Double", this);
		FillMapFromYaml<int>(node, "Ints", this);
		FillMapFromYaml<float>(node, "Floats", this);
		FillMapFromYaml<Vec2f>(node, "Vec2fs", this);
		FillMapFromYaml<Vec3f>(node, "Vec3fs", this);
		FillMapFromYaml<Vec4f>(node, "Vec4fs", this);
		FillMapFromYaml<Vec2d>(node, "Vec2ds", this);
		FillMapFromYaml<Vec3d>(node, "Vec3ds", this);
		FillMapFromYaml<Vec4d>(node, "Vec4ds", this);
		FillMapFromYaml<Vec2i>(node, "Vec2is", this);
		FillMapFromYaml<Vec3i>(node, "Vec2is", this);
		FillMapFromYaml<Vec4i>(node, "Vec2is", this);
		FillMapFromYaml<Mat2x2f>(node, "Mat2x2fs", this);
		FillMapFromYaml<Mat3x3f>(node, "Mat3x3fs", this);
		FillMapFromYaml<Mat4x4f>(node, "Mat4x4fs", this);
		FillMapFromYaml<Mat2x2d>(node, "Mat2x2ds", this);
		FillMapFromYaml<Mat3x3d>(node, "Mat3x3ds", this);
		FillMapFromYaml<Mat4x4d>(node, "Mat4x4ds", this);

		if (node["Textures"])
		{
			for (const auto& it : node["Textures"])
			{
				ObjectAssetEntry entry = it.second.as<ObjectAssetEntry>();
				Texture* tex = AssetsRegistry::GetInstance().GetObjectFromEntry<Texture>(entry);
				SetTexture(it.first.as<std::string>(), tex);
			}
		}

	}
}
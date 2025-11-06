#include "Twisted/Rendering/Material.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/Shader.h"

namespace Twisted
{
	Material::Material(const std::string& name) :TObject(name) {}

	Material::Material(const std::string& name, const MaterialData& materialData) :
		TObject(name),
		m_data(materialData)
	{
	}

	YAML::Node Material::YAMLSerialize()const
	{
		YAML::Node serNode= m_data.SerializeMaterialData();
		serNode["shader"] = YamlUtils::encodeTObject(m_shader);
		return serNode;
	}

	void Material::YAMLDeserialize(const YAML::Node& data)
	{
		m_data.DeserializeMaterialData(data);

		if (data["shader"])
			m_shader = static_cast<Shader*>(YamlUtils::decodeTObject(data["shader"]));
	}

	YAML::Node MaterialData::SerializeMaterialData()const
	{
		YAML::Node node;

		auto serializeMap = [&](auto& map, const std::string& name)
			{
				if (map.empty()) return;
				YAML::Node cat;
				for (auto& [key, value] : map)
					cat[key] = value;
				node[name] = cat;
			};

		serializeMap(UnsignedInts, "UnsignedInts");
		serializeMap(Bools, "Bools");
		serializeMap(Ints, "Ints");
		serializeMap(Floats, "Floats");
		serializeMap(Doubles, "Doubles");
		serializeMap(Vec4fs, "Vec4fs");
		serializeMap(Vec3fs, "Vec3fs");
		serializeMap(Vec2fs, "Vec2fs");
		serializeMap(Vec4ds, "Vec4ds");
		serializeMap(Vec3ds, "Vec3ds");
		serializeMap(Vec2ds, "Vec2ds");
		serializeMap(Vec4is, "Vec4is");
		serializeMap(Vec3is, "Vec3is");
		serializeMap(Vec2is, "Vec2is");
		serializeMap(Mat4x4fs, "Mat4x4fs");
		serializeMap(Mat3x3fs, "Mat3x3fs");
		serializeMap(Mat2x2fs, "Mat2x2fs");
		serializeMap(Mat4x4ds, "Mat4x4ds");
		serializeMap(Mat3x3ds, "Mat3x3ds");
		serializeMap(Mat2x2ds, "Mat2x2ds");

		// For textures, we assume WPtr<Texture> can be represented as a string (like a path or ID)
		if (!Textures.empty())
		{
			YAML::Node texNode;
			for (auto& [key, tex] : Textures)
			{
				// For now, store as string ID or placeholder
				texNode[key] = YamlUtils::encodeTObject(tex.GetObj());
			}
			node["Textures"] = texNode;
		}

		return node;
	}

	void MaterialData::DeserializeMaterialData(const YAML::Node& node)
	{
		auto deserializeMap = [&](auto& map, const std::string& name)
			{
				if (!node[name]) return;
				for (auto it : node[name])
					map[it.first.as<std::string>()] = it.second.as<typename std::decay_t<decltype(map.begin()->second)>>();
			};

		deserializeMap(UnsignedInts, "UnsignedInts");
		deserializeMap(Bools, "Bools");
		deserializeMap(Ints, "Ints");
		deserializeMap(Floats, "Floats");
		deserializeMap(Doubles, "Doubles");
		deserializeMap(Vec4fs, "Vec4fs");
		deserializeMap(Vec3fs, "Vec3fs");
		deserializeMap(Vec2fs, "Vec2fs");
		deserializeMap(Vec4ds, "Vec4ds");
		deserializeMap(Vec3ds, "Vec3ds");
		deserializeMap(Vec2ds, "Vec2ds");
		deserializeMap(Vec4is, "Vec4is");
		deserializeMap(Vec3is, "Vec3is");
		deserializeMap(Vec2is, "Vec2is");
		deserializeMap(Mat4x4fs, "Mat4x4fs");
		deserializeMap(Mat3x3fs, "Mat3x3fs");
		deserializeMap(Mat2x2fs, "Mat2x2fs");
		deserializeMap(Mat4x4ds, "Mat4x4ds");
		deserializeMap(Mat3x3ds, "Mat3x3ds");
		deserializeMap(Mat2x2ds, "Mat2x2ds");

		YAML::Node texNode = node["Textures"];
		if (texNode && texNode.IsMap())
		{
			for (auto it = texNode.begin(); it != texNode.end(); ++it)
			{
				std::string key = it->first.as<std::string>();
				Texture* tex = static_cast<Texture*>(YamlUtils::decodeTObject(it->second));

				Textures[key] = tex;
			}
		}
	}

	void Material::ApplyUniforms()
	{
		if (!m_shader)
			return;

		for (const auto& uniform : m_shader->GetUniforms())
		{
			switch (uniform.Type)
			{
			case ShaderVarType::BOOL:
				m_shader->SetVar(uniform.UniformID, Get<bool>(uniform.Name));
				break;
			case ShaderVarType::UNSIGNED_INT:
				m_shader->SetVar(uniform.UniformID, Get<unsigned int>(uniform.Name));
				break;
			case ShaderVarType::INT:
				m_shader->SetVar(uniform.UniformID, Get<int>(uniform.Name));
				break;
			case ShaderVarType::FLOAT:
				m_shader->SetVar(uniform.UniformID, Get<float>(uniform.Name));
				break;
			case ShaderVarType::VEC2_F:
				m_shader->SetVar(uniform.UniformID, Get<Vec2f>(uniform.Name));
				break;
			case ShaderVarType::VEC3_F:
				m_shader->SetVar(uniform.UniformID, Get<Vec3f>(uniform.Name));
				break;
			case ShaderVarType::VEC4_F:
				m_shader->SetVar(uniform.UniformID, Get<Vec4f>(uniform.Name));
				break;
			case ShaderVarType::VEC2_D:
				m_shader->SetVar(uniform.UniformID, Get<Vec2d>(uniform.Name));
				break;
			case ShaderVarType::VEC3_D:
				m_shader->SetVar(uniform.UniformID, Get<Vec3d>(uniform.Name));
				break;
			case ShaderVarType::VEC4_D:
				m_shader->SetVar(uniform.UniformID, Get<Vec4d>(uniform.Name));
				break;
			case ShaderVarType::VEC2_I:
				m_shader->SetVar(uniform.UniformID, Get<Vec2i>(uniform.Name));
				break;
			case ShaderVarType::VEC3_I:
				m_shader->SetVar(uniform.UniformID, Get<Vec3i>(uniform.Name));
				break;
			case ShaderVarType::VEC4_I:
				m_shader->SetVar(uniform.UniformID, Get<Vec4i>(uniform.Name));
				break;
			case ShaderVarType::MAT4x4_F:
				m_shader->SetVar(uniform.UniformID, Get<Mat4x4f>(uniform.Name));
				break;
			case ShaderVarType::MAT3x3_F:
				m_shader->SetVar(uniform.UniformID, Get<Mat3x3f>(uniform.Name));
				break;
			case ShaderVarType::MAT2x2_F:
				m_shader->SetVar(uniform.UniformID, Get<Mat2x2f>(uniform.Name));
				break;
			case ShaderVarType::MAT4x4_D:
				m_shader->SetVar(uniform.UniformID, Get<Mat4x4d>(uniform.Name));
				break;
			case ShaderVarType::MAT3x3_D:
				m_shader->SetVar(uniform.UniformID, Get<Mat3x3d>(uniform.Name));
				break;
			case ShaderVarType::MAT2x2_D:
				m_shader->SetVar(uniform.UniformID, Get<Mat2x2d>(uniform.Name));
				break;
			case ShaderVarType::SAMPLER2D:
			{
				auto tex = Get<WPtr<Texture>>(uniform.Name);
				unsigned int texID = tex ? tex->GetTexID() : 0;
				m_shader->SetTex(uniform.UniformID, texID, uniform.TextureUnit);
				break;
			}
			default:
				TWISTED_WARN("Unsupported shader uniform type");
				break;
			}
		}
	}

}
#include "Twisted/Rendering/Material.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/Rendering/Shader.h"
#include <yaml-cpp/node/node.h>
#include <Utils/YamlUtils.h>
#include "Utils/WPtr.h"
#include <variant>
#include <string>
#include <type_traits>

#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"

namespace Twisted
{
	const std::string PROP_TYPE_KEY = "type";
	const std::string PROP_VALUE_KEY = "value";
	const std::string PROPERTIES_KEY = "properties";
	const std::string SHADER_KEY = "shader";
	const std::string TEXTURE_TYPE_NAME = "tex";

	void Material::ApplyUniforms()
	{
		if (!m_shader)
			return;

		for (auto& uniform : m_shader->Uniforms)
		{
			auto it = m_properties.find(uniform.Name);
			if (it == m_properties.end())
				continue;

			switch (uniform.Type)
			{
			case ShaderVarType::BOOL:
				Shader_GL::SetVar(uniform.UniformID, Get<bool>(uniform.Name, false));
				break;
			case ShaderVarType::INT:
				Shader_GL::SetVar(uniform.UniformID, Get<int>(uniform.Name, 0));
				break;
			case ShaderVarType::FLOAT:
				Shader_GL::SetVar(uniform.UniformID, Get<float>(uniform.Name, 0.0f));
				break;
			case ShaderVarType::VEC2_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Vec2f>(uniform.Name, Vec3f{ 1.0f,1.0f,1.0f }));
				break;
			case ShaderVarType::VEC3_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Vec3f>(uniform.Name, Vec3f{ 1.0f,1.0f,1.0f }));
				break;
			case ShaderVarType::VEC4_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Vec4f>(uniform.Name, Vec4f{ 1.0f,1.0f,1.0f,1.0f }));
				break;
			case ShaderVarType::MAT4x4_F:
				Shader_GL::SetVar(uniform.UniformID, Get<Mat4x4f>(uniform.Name, Mat4x4f{ 1.0f }));
				break;
			case ShaderVarType::SAMPLER2D:
			{
				auto tex = Get<WPtr<Texture>>(uniform.Name, nullptr);
				if (tex)
					Shader_GL::SetTex(uniform.UniformID, uniform.TextureUnit, tex->TexID);
				else
					Shader_GL::SetTex(uniform.UniformID, uniform.TextureUnit, 0);
				break;
			}
			}
		}
	}

	YAML::Node Material::YAMLSerialize() const //TODO
	{
		YAML::Node node;
		node[SHADER_KEY] = YamlUtils::encodeTObject(m_shader);

		YAML::Node props = node[PROPERTIES_KEY];
		for (const auto& [name, value] : m_properties)
		{
			YAML::Node entry = props[name];
			// Save material properties
			std::visit([&](auto&& v)
				{
					using T = std::decay_t<decltype(v)>;

					// TEXTURE SPECIAL CASE
					if constexpr (std::is_same_v<T, WPtr<Texture>>)
					{
						entry[PROP_TYPE_KEY] = TEXTURE_TYPE_NAME;

						const Texture* tex = v.get();
						entry[PROP_VALUE_KEY] = YamlUtils::encodeTObject(tex);
					}
					else
					{
						// Everything else supports YAML encode
						entry[PROP_TYPE_KEY] = typeid(T).name();   // You may replace this with your own enum/string
						entry[PROP_VALUE_KEY] = v;                 // relies on existing YAML conversions
					}
				}, value
			);
		}
		return node;
	}
	void Material::YAMLDeserialize(const YAML::Node& node) //TODO
	{
		if (node[PROPERTIES_KEY])
		{
			const auto propertiesNode = node[PROPERTIES_KEY];
			for (auto it = propertiesNode.begin(); it != propertiesNode.end(); ++it)
			{
				const std::string& key = it->first.as<std::string>();
				const YAML::Node& entry = it->second;

				const std::string typeStr = entry[PROP_TYPE_KEY].as<std::string>();
				const YAML::Node encodedValue = entry[PROP_VALUE_KEY];

				if (typeStr == TEXTURE_TYPE_NAME)
				{
					Texture* tex = static_cast<Texture*>(YamlUtils::decodeTObject(encodedValue));
					m_properties[key] = WPtr<Texture>(tex);
				}
				else if (typeStr == typeid(bool).name())
					m_properties[key] = encodedValue.as<bool>();
				else if (typeStr == typeid(int).name())
					m_properties[key] = encodedValue.as<int>();
				else if (typeStr == typeid(unsigned int).name())
					m_properties[key] = encodedValue.as<unsigned int>();
				else if (typeStr == typeid(float).name())
					m_properties[key] = encodedValue.as<float>();
				else if (typeStr == typeid(double).name())
					m_properties[key] = encodedValue.as<double>();
				else if (typeStr == typeid(Vec2f).name())
					m_properties[key] = encodedValue.as<Vec2f>();
				else if (typeStr == typeid(Vec3f).name())
					m_properties[key] = encodedValue.as<Vec3f>();
				else if (typeStr == typeid(Vec4f).name())
					m_properties[key] = encodedValue.as<Vec4f>();
				else if (typeStr == typeid(Vec2d).name())
					m_properties[key] = encodedValue.as<Vec2d>();
				else if (typeStr == typeid(Vec3d).name())
					m_properties[key] = encodedValue.as<Vec3d>();
				else if (typeStr == typeid(Vec4d).name())
					m_properties[key] = encodedValue.as<Vec4d>();
				else if (typeStr == typeid(Vec2i).name())
					m_properties[key] = encodedValue.as<Vec2i>();
				else if (typeStr == typeid(Vec3i).name())
					m_properties[key] = encodedValue.as<Vec3i>();
				else if (typeStr == typeid(Vec4i).name())
					m_properties[key] = encodedValue.as<Vec4i>();
				else if (typeStr == typeid(Mat2x2f).name())
					m_properties[key] = encodedValue.as<Mat2x2f>();
				else if (typeStr == typeid(Mat3x3f).name())
					m_properties[key] = encodedValue.as<Mat3x3f>();
				else if (typeStr == typeid(Mat4x4f).name())
					m_properties[key] = encodedValue.as<Mat4x4f>();
				else if (typeStr == typeid(Mat2x2d).name())
					m_properties[key] = encodedValue.as<Mat2x2d>();
				else if (typeStr == typeid(Mat3x3d).name())
					m_properties[key] = encodedValue.as<Mat3x3d>();
				else if (typeStr == typeid(Mat4x4d).name())
					m_properties[key] = encodedValue.as<Mat4x4d>();
				else
				{
					TWISTED_WARN("Unknown property type: " + typeStr);
				}
			}
		}
		if (node[SHADER_KEY])
			m_shader = static_cast<Shader*>(YamlUtils::decodeTObject(node[SHADER_KEY]));
	}
}


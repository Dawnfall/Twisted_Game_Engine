#pragma once

#include "Utils/glmUtils.h"
#include "Utils/WPtr.h"
#include "Twisted/Rendering/Texture.h"
#include "Twisted/Rendering/Shader.h"
#include "Utils/YamlUtils.h"

#include "Debug/Logger.h"

#include <yaml-cpp/node/node.h>
#include <unordered_map>
#include <string>
#include <tuple>
#include <variant>

namespace Twisted
{
	inline const std::string PROP_TYPE_KEY = "type";
	inline const std::string PROP_VALUE_KEY = "value";
	inline const std::string PROPERTIES_KEY = "properties";
	inline const std::string SHADER_KEY = "shader";
	inline const std::string TEXTURE_TYPE_NAME = "tex";

	template<typename T>
	concept MaterialSupportedType =
		std::same_as<T, bool>
		|| std::same_as<T, int>
		|| std::same_as<T, float>
		|| std::same_as<T, Vec2f>
		|| std::same_as<T, Vec3f>
		|| std::same_as<T, Vec4f>
		|| std::same_as<T, Mat2x2f>
		|| std::same_as <T, Mat3x3f>
		|| std::same_as<T, Mat4x4f>
		|| std::same_as <T, Texture*>;

	struct MaterialValue
	{
		MaterialValue(ShaderValueType val,const std::string& name, size_t reflectionIndex):
			val(val),
			name(name),
			reflectionIndex(reflectionIndex)
		{ }

		ShaderValueType val;
		size_t reflectionIndex = 0;
		std::string name;
		int version = 0;
	};

	class Shader;
	class TWISTED_API Material :public TObject
	{
	public:
		Material(const std::string& name) :TObject(name) {}

		Shader* GetShader() { return m_shader; }
		const Shader* GetShader() const { return m_shader; }
		void SetShader(Shader* shader);

		const std::vector<MaterialValue>& GetValues()const { return m_values; }
		std::vector<MaterialValue>& GetValues() { return m_values; }

		template<MaterialSupportedType T>
		T* TryGet(const std::string& name)
		{
			auto it = m_nameToIndex.find(name);
			if (it == m_nameToIndex.end())
				return nullptr;

			auto& mv = m_values.at(it->second);
			return std::get_if<T>(mv->val);
		}
		template<MaterialSupportedType T>
		const T* TryGet(const std::string& name) const
		{
			auto it = m_nameToIndex.find(name);
			if (it == m_nameToIndex.end())
				return nullptr;

			auto& mv = m_values.at(it->second);
			return std::get_if<T>(mv->val);
		}

		template<MaterialSupportedType T>
		bool Set(const std::string& name, T&& v)
		{
			auto it = m_nameToIndex.find(name);
			if (it == m_nameToIndex.end())
				return false;

			MaterialValue& currValue = m_values.at(it->second);
			return std::visit([&](auto& newValue)->bool {
				using X = std::decay_t<decltype(newValue)>;
				if constexpr (std::is_same_v<X, std::decay_t<T>>) {
					newValue = std::forward<T>(v);
					return true;
				}
				else
					return false;
				}, currValue);
		}

		bool Clear(const std::string& name);

		void ApplyUniforms();

	private:
		std::vector<MaterialValue> m_values;
		std::unordered_map<std::string, size_t> m_nameToIndex;

		Shader* m_shader = nullptr;
	};

	template<>
	inline YAML::Node YamlSerialize<Material>(const Material& material)
	{
		YAML::Node node;
		node[SHADER_KEY] = YamlUtils::encodeTObject(material.GetShader());

		//YAML::Node props = node[PROPERTIES_KEY];
		//for (const auto& activeUniformValue : material.GetActiveUniforms())
		//{
		//	YAML::Node entry = props[name];
		//	// Save material properties
		//	std::visit([&](auto&& v)
		//		{
		//			using T = std::decay_t<decltype(v)>;

		//			// TEXTURE SPECIAL CASE
		//			if constexpr (std::is_same_v<T, WPtr<Texture>>)
		//			{
		//				entry[PROP_TYPE_KEY] = TEXTURE_TYPE_NAME;

		//				const Texture* tex = v.get();
		//				entry[PROP_VALUE_KEY] = YamlUtils::encodeTObject(tex);
		//			}
		//			else
		//			{
		//				// Everything else supports YAML encode
		//				entry[PROP_TYPE_KEY] = typeid(T).name();   // You may replace this with your own enum/string
		//				entry[PROP_VALUE_KEY] = v;                 // relies on existing YAML conversions
		//			}
		//		}, value
		//	);
		//}
		return node;
	}
	template<>
	inline void YamlDeserialize<Material>(Material&, const YAML::Node&)
	{
		/*if (node[PROPERTIES_KEY])
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
					material.Values[key] = WPtr<Texture>(tex);
				}
				else if (typeStr == typeid(bool).name())
					material.Values[key] = encodedValue.as<bool>();
				else if (typeStr == typeid(int).name())
					material.Values[key] = encodedValue.as<int>();
				else if (typeStr == typeid(unsigned int).name())
					material.Values[key] = encodedValue.as<unsigned int>();
				else if (typeStr == typeid(float).name())
					material.Values[key] = encodedValue.as<float>();
				else if (typeStr == typeid(double).name())
					material.Values[key] = encodedValue.as<double>();
				else if (typeStr == typeid(Vec2f).name())
					material.Values[key] = encodedValue.as<Vec2f>();
				else if (typeStr == typeid(Vec3f).name())
					material.Values[key] = encodedValue.as<Vec3f>();
				else if (typeStr == typeid(Vec4f).name())
					material.Values[key] = encodedValue.as<Vec4f>();
				else if (typeStr == typeid(Vec2d).name())
					material.Values[key] = encodedValue.as<Vec2d>();
				else if (typeStr == typeid(Vec3d).name())
					material.Values[key] = encodedValue.as<Vec3d>();
				else if (typeStr == typeid(Vec4d).name())
					material.Values[key] = encodedValue.as<Vec4d>();
				else if (typeStr == typeid(Vec2i).name())
					material.Values[key] = encodedValue.as<Vec2i>();
				else if (typeStr == typeid(Vec3i).name())
					material.Values[key] = encodedValue.as<Vec3i>();
				else if (typeStr == typeid(Vec4i).name())
					material.Values[key] = encodedValue.as<Vec4i>();
				else if (typeStr == typeid(Mat2x2f).name())
					material.Values[key] = encodedValue.as<Mat2x2f>();
				else if (typeStr == typeid(Mat3x3f).name())
					material.Values[key] = encodedValue.as<Mat3x3f>();
				else if (typeStr == typeid(Mat4x4f).name())
					material.Values[key] = encodedValue.as<Mat4x4f>();
				else if (typeStr == typeid(Mat2x2d).name())
					material.Values[key] = encodedValue.as<Mat2x2d>();
				else if (typeStr == typeid(Mat3x3d).name())
					material.Values[key] = encodedValue.as<Mat3x3d>();
				else if (typeStr == typeid(Mat4x4d).name())
					material.Values[key] = encodedValue.as<Mat4x4d>();
				else
				{
					TWISTED_WARN("Unknown property type: " + typeStr);
				}
			}
		}
		if (node[SHADER_KEY])
		{
			material.SetShader(static_cast<Shader*>(YamlUtils::decodeTObject(node[SHADER_KEY])));
		}*/
	}
}
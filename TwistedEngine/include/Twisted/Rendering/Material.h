#pragma once

#include "Utils/GlmUtils.h"
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
			reflectionIndex(reflectionIndex),
			name(name)
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
			return std::get_if<T>(&mv.val);
		}
		template<MaterialSupportedType T>
		const T* TryGet(const std::string& name) const
		{
			auto it = m_nameToIndex.find(name);
			if (it == m_nameToIndex.end())
				return nullptr;

			auto& mv = m_values.at(it->second);
			return std::get_if<T>(&mv.val);
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

		YAML::Node props;
		for (const auto& mv : material.GetValues())
		{
			YAML::Node entry;
			bool handled = true;
			std::visit([&](const auto& val) {
				using T = std::decay_t<decltype(val)>;
				if constexpr (std::is_same_v<T, bool>) {
					entry[PROP_TYPE_KEY] = "bool";
					entry[PROP_VALUE_KEY] = val;
				} else if constexpr (std::is_same_v<T, int>) {
					entry[PROP_TYPE_KEY] = "int";
					entry[PROP_VALUE_KEY] = val;
				} else if constexpr (std::is_same_v<T, float>) {
					entry[PROP_TYPE_KEY] = "float";
					entry[PROP_VALUE_KEY] = val;
				} else if constexpr (std::is_same_v<T, Vec3f>) {
					entry[PROP_TYPE_KEY] = "vec3f";
					entry[PROP_VALUE_KEY] = val;
				} else if constexpr (std::is_same_v<T, Vec4f>) {
					entry[PROP_TYPE_KEY] = "vec4f";
					entry[PROP_VALUE_KEY] = val;
				} else if constexpr (std::is_same_v<T, TextureValue>) {
					entry[PROP_TYPE_KEY] = TEXTURE_TYPE_NAME;
					entry[PROP_VALUE_KEY] = YamlUtils::encodeTObject(val.tex.get());
				} else {
					handled = false;
				}
			}, mv.val);
			if (handled)
				props[mv.name] = entry;
		}
		node[PROPERTIES_KEY] = props;
		return node;
	}

	template<>
	inline void YamlDeserialize<Material>(Material& material, const YAML::Node& node)
	{
		if (node[SHADER_KEY])
			material.SetShader(static_cast<Shader*>(YamlUtils::decodeTObject(node[SHADER_KEY])));

		if (!node[PROPERTIES_KEY])
			return;

		const auto& props = node[PROPERTIES_KEY];
		for (auto& mv : material.GetValues())
		{
			if (!props[mv.name])
				continue;

			const auto& entry = props[mv.name];
			if (!entry[PROP_TYPE_KEY] || !entry[PROP_VALUE_KEY])
				continue;

			const std::string typeStr = entry[PROP_TYPE_KEY].as<std::string>();
			const auto& valNode = entry[PROP_VALUE_KEY];

			if (typeStr == "bool")
				mv.val = valNode.as<bool>();
			else if (typeStr == "int")
				mv.val = valNode.as<int>();
			else if (typeStr == "float")
				mv.val = valNode.as<float>();
			else if (typeStr == "vec3f")
				mv.val = valNode.as<Vec3f>();
			else if (typeStr == "vec4f")
				mv.val = valNode.as<Vec4f>();
			else if (typeStr == TEXTURE_TYPE_NAME)
			{
				TextureValue tv;
				tv.tex = WPtr<Texture>(static_cast<Texture*>(YamlUtils::decodeTObject(valNode)));
				mv.val = tv;
			}
		}
	}
}
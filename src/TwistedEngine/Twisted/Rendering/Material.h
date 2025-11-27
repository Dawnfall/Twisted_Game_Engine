#pragma once

#include <variant>
#include <string>
#include <unordered_map>
#include "Utils/glmUtils.h"
#include "Utils/WPtr.h"
#include "Twisted/Rendering/Texture.h"
#include "Shader.h"
#include <yaml-cpp/node/node.h>
#include <tuple>
#include "Utils/YamlUtils.h"

#include "Debug/Logger.h"


using MaterialValue = std::variant<
	bool, int, unsigned int, float, double,
	Vec2f, Vec3f, Vec4f,
	Vec2d, Vec3d, Vec4d,
	Vec2i, Vec3i, Vec4i,
	Mat2x2f, Mat3x3f, Mat4x4f,
	Mat2x2d, Mat3x3d, Mat4x4d,
	Twisted::WPtr<Twisted::Texture>
>;

namespace Twisted
{
	inline const std::string PROP_TYPE_KEY = "type";
	inline const std::string PROP_VALUE_KEY = "value";
	inline const std::string PROPERTIES_KEY = "properties";
	inline const std::string SHADER_KEY = "shader";
	inline const std::string TEXTURE_TYPE_NAME = "tex";

	class TWISTED_API Material :public TObject
	{
	public:
		Material(const std::string& name) :TObject(name) {}

		template<typename T>
		void Set(const std::string& name, T value)
		{
			Properties[name] = std::move(value);
		}

		// Get a value (with default)
		template<typename T>
		std::optional<T> Get(const std::string& name) const
		{
			auto it = Properties.find(name);
			if (it != Properties.end())
				if (auto val = std::get_if<T>(&it->second))
					return *val;

			return std::nullopt;
		}

		template<typename T>
		T Get(const std::string& name, const T& defaultValue)
		{
			auto it = Properties.find(name);
			if (it != Properties.end())
				if (auto val = std::get_if<T>(&it->second))
					return *val;

			return defaultValue;
		}

		void Clear()
		{
			Properties.clear();
			Shader = nullptr;
		}

		void ApplyUniforms();

		std::unordered_map<std::string, MaterialValue> Properties;
		Shader* Shader = nullptr;
	};

	template<>
	inline YAML::Node YamlSerialize<Material>(const Material& material)
	{
		YAML::Node node;
		node[SHADER_KEY] = YamlUtils::encodeTObject(material.Shader);

		YAML::Node props = node[PROPERTIES_KEY];
		for (const auto& [name, value] : material.Properties)
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
	template<>
	inline void YamlDeserialize<Material>(Material& material, const YAML::Node& node)
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
					material.Properties[key] = WPtr<Texture>(tex);
				}
				else if (typeStr == typeid(bool).name())
					material.Properties[key] = encodedValue.as<bool>();
				else if (typeStr == typeid(int).name())
					material.Properties[key] = encodedValue.as<int>();
				else if (typeStr == typeid(unsigned int).name())
					material.Properties[key] = encodedValue.as<unsigned int>();
				else if (typeStr == typeid(float).name())
					material.Properties[key] = encodedValue.as<float>();
				else if (typeStr == typeid(double).name())
					material.Properties[key] = encodedValue.as<double>();
				else if (typeStr == typeid(Vec2f).name())
					material.Properties[key] = encodedValue.as<Vec2f>();
				else if (typeStr == typeid(Vec3f).name())
					material.Properties[key] = encodedValue.as<Vec3f>();
				else if (typeStr == typeid(Vec4f).name())
					material.Properties[key] = encodedValue.as<Vec4f>();
				else if (typeStr == typeid(Vec2d).name())
					material.Properties[key] = encodedValue.as<Vec2d>();
				else if (typeStr == typeid(Vec3d).name())
					material.Properties[key] = encodedValue.as<Vec3d>();
				else if (typeStr == typeid(Vec4d).name())
					material.Properties[key] = encodedValue.as<Vec4d>();
				else if (typeStr == typeid(Vec2i).name())
					material.Properties[key] = encodedValue.as<Vec2i>();
				else if (typeStr == typeid(Vec3i).name())
					material.Properties[key] = encodedValue.as<Vec3i>();
				else if (typeStr == typeid(Vec4i).name())
					material.Properties[key] = encodedValue.as<Vec4i>();
				else if (typeStr == typeid(Mat2x2f).name())
					material.Properties[key] = encodedValue.as<Mat2x2f>();
				else if (typeStr == typeid(Mat3x3f).name())
					material.Properties[key] = encodedValue.as<Mat3x3f>();
				else if (typeStr == typeid(Mat4x4f).name())
					material.Properties[key] = encodedValue.as<Mat4x4f>();
				else if (typeStr == typeid(Mat2x2d).name())
					material.Properties[key] = encodedValue.as<Mat2x2d>();
				else if (typeStr == typeid(Mat3x3d).name())
					material.Properties[key] = encodedValue.as<Mat3x3d>();
				else if (typeStr == typeid(Mat4x4d).name())
					material.Properties[key] = encodedValue.as<Mat4x4d>();
				else
				{
					TWISTED_WARN("Unknown property type: " + typeStr);
				}
			}
		}
		if (node[SHADER_KEY])
		{
			material.Shader = static_cast<Shader*>(YamlUtils::decodeTObject(node[SHADER_KEY]));
		}
	}
}
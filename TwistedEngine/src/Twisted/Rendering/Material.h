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

//TODO....
//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	class TWISTED_API Material :public TObject
	{
	public:
		Material(const std::string& name) :TObject(name) {}

		template<typename T>
		void Set(const std::string& name, T value)
		{
			m_properties[name] = std::move(value);
		}

		// Get a value (with default)
		template<typename T>
		std::optional<T> Get(const std::string& name) const
		{
			auto it = m_properties.find(name);
			if (it != m_properties.end())
				if (auto val = std::get_if<T>(&it->second))
					return *val;

			return std::nullopt;
		}

		template<typename T>
		T Get(const std::string& name, const T& defaultValue)
		{
			auto it = m_properties.find(name);
			if (it != m_properties.end())
				if (auto val = std::get_if<T>(&it->second))
					return *val;

			return defaultValue;
		}

		void Clear()
		{
			m_properties.clear();
			m_shader = nullptr;
		}
		void SetShader(Shader* shader)
		{
			m_shader = shader;
		}
		Shader* GetShader() { return m_shader; }

		YAML::Node YAMLSerialize()const;
		void YAMLDeserialize(const YAML::Node& node);

		void ApplyUniforms();


	private:
		std::unordered_map<std::string, MaterialValue> m_properties;
		Shader* m_shader = nullptr;
	};
}
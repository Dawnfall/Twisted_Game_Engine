#pragma once
#include "AppCore.h"
#include "Shader.h"
#include "Utils/GlmUtils.h"
#include "Texture.h"


#include <unordered_map>

namespace Twisted
{
	template<typename T>
	struct MaterialValue
	{
		int ID = -1;
		T value = T();
	};

	class Material
	{
	public:
		void SetShader(const SRef<Shader>& shader)
		{
			if (shader == nullptr || !shader->IsValid() || m_shader == shader)
				return;

			if (m_shader != nullptr)
			{
				m_bools.clear();
				m_ints.clear();
				m_floats.clear();
				m_vec2s.clear();
				m_vec3s.clear();
				m_vec4s.clear();
				m_mats.clear();
			}

			m_shader = shader;
			if (m_shader != nullptr)
				PresetUniforms();
		}
		const SRef<Shader> GetShader()const { return m_shader; }

		//TODO... check first
		void SetBool(const std::string& varName, bool value) { m_bools[varName].value = value; }
		void SetInt(const std::string& varName, int value) { m_ints[varName].value = value; }
		void SetFloat(const std::string& varName, float value) { m_floats[varName].value = value; }
		void SetVec2f(const std::string& varName, Vec2f value) { m_vec2s[varName].value = value; }
		void SetVec3f(const std::string& varName, Vec3f value) { m_vec3s[varName].value = value; }
		void SetVec4f(const std::string& varName, Vec4f value) { m_vec4s[varName].value = value; }
		void SetMat4x4f(const std::string& varName, Mat4x4f value) { m_mats[varName].value = value; }
		void SetTexture(const std::string& shadervarName, const SRef<Texture>& texture)
		{
			int locationID = m_shaderTexVarToIdMap[shadervarName];
			m_textures[locationID] = texture;
		}
		
		void ApplyUniforms()const;

		std::unordered_map<std::string, MaterialValue<bool>> m_bools;
		std::unordered_map<std::string, MaterialValue<int>> m_ints;
		std::unordered_map<std::string, MaterialValue<float>> m_floats;
		std::unordered_map<std::string, MaterialValue<Vec4f>> m_vec4s;
		std::unordered_map<std::string, MaterialValue<Vec3f>> m_vec3s;
		std::unordered_map<std::string, MaterialValue<Vec2f>> m_vec2s;
		std::unordered_map<std::string, MaterialValue<Mat4x4f>> m_mats;

		std::unordered_map<int, SRef<Texture>> m_textures;
		std::unordered_map<std::string, int> m_shaderTexVarToIdMap;

	private:
		void PresetUniforms();

		SRef<Shader> m_shader = nullptr;
	};
}
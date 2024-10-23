#pragma once

#include "pch.h"

#include "Debug/Logger.h"
#include "Shader.h"
#include "Collections/Geometry.h"
#include "Texture.h"

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
		void SetShader(const std::shared_ptr<Shader>& shader)
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
		const std::shared_ptr<Shader> GetShader()const { return m_shader; }

		//TODO... check first
		void SetBool(const std::string& varName, bool value) { m_bools[varName].value = value; }
		void SetInt(const std::string& varName, int value) { m_ints[varName].value = value; }
		void SetFloat(const std::string& varName, float value) { m_floats[varName].value = value; }
		void SetVec2f(const std::string& varName, Vec2f value) { m_vec2s[varName].value = value; }
		void SetVec3f(const std::string& varName, Vec3f value) { m_vec3s[varName].value = value; }
		void SetVec4f(const std::string& varName, Vec4f value) { m_vec4s[varName].value = value; }
		void SetMat4x4f(const std::string& varName, Mat4x4f value) { m_mats[varName].value = value; }
		void SetTexture(const std::string& shadervarName, const std::shared_ptr<Texture>& texture)
		{
			int locationID = m_shaderTexVarToIdMap[shadervarName];
			m_textures[locationID] = texture;
		}

		std::unordered_map<std::string, MaterialValue<bool>> m_bools;
		std::unordered_map<std::string, MaterialValue<int>> m_ints;
		std::unordered_map<std::string, MaterialValue<float>> m_floats;
		std::unordered_map<std::string, MaterialValue<Vec4f>> m_vec4s;
		std::unordered_map<std::string, MaterialValue<Vec3f>> m_vec3s;
		std::unordered_map<std::string, MaterialValue<Vec2f>> m_vec2s;
		std::unordered_map<std::string, MaterialValue<Mat4x4f>> m_mats;

		std::unordered_map<int, std::shared_ptr<Texture>> m_textures;
		std::unordered_map<std::string, int> m_shaderTexVarToIdMap;
	private:
		std::shared_ptr<Shader> m_shader = nullptr;

		void PresetUniforms()
		{
			for (auto& uniVar : m_shader->Uniforms)
			{
				switch (uniVar.Type)
				{
				case GL_SAMPLER_2D:
					m_shaderTexVarToIdMap[uniVar.Name] = uniVar.UniformID;
					break;
				case GL_BOOL:
					m_bools[uniVar.Name] = MaterialValue<bool>{ uniVar.UniformID };
					break;
				case GL_FLOAT:
					m_floats[uniVar.Name] = MaterialValue<float>{ uniVar.UniformID };
					break;
				case GL_INT:
					m_ints[uniVar.Name] = MaterialValue<int>{ uniVar.UniformID };
					break;
				case GL_FLOAT_VEC2:
					m_vec2s[uniVar.Name] = MaterialValue<Vec2f>{ uniVar.UniformID };
					break;
				case GL_FLOAT_VEC3:
					m_vec3s[uniVar.Name] = MaterialValue<Vec3f>{ uniVar.UniformID };
					break;
				case GL_FLOAT_VEC4:
					m_vec4s[uniVar.Name] = MaterialValue<Vec4f>{ uniVar.UniformID };
					break;
				case GL_FLOAT_MAT4:
					m_mats[uniVar.Name] = MaterialValue<Mat4x4f>{ uniVar.UniformID };
					break;
				default:
					//DAWN_INFO("Unsupported shader uniform var type");
					break;
				}
			}
		}
	};
}
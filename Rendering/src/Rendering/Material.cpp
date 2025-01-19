#include "renderingpch.h"
#include "Material.h"

namespace Twisted
{
	void Material::ApplyUniforms()const
	{
		for (auto& var : m_bools)
			m_shader->SetVar(var.second.ID, var.second.value);
		for (auto& var : m_ints)
			m_shader->SetVar(var.second.ID, var.second.value);
		for (auto& var : m_floats)
			m_shader->SetVar(var.second.ID, var.second.value);
		for (auto& var : m_vec2s)
			m_shader->SetVar(var.second.ID, var.second.value);
		for (auto& var : m_vec3s)
			m_shader->SetVar(var.second.ID, var.second.value);
		for (auto& var : m_vec4s)
			m_shader->SetVar(var.second.ID, var.second.value);
		for (auto& var : m_mats)
			m_shader->SetVar(var.second.ID, var.second.value);
		for (auto& var : m_textures)
			m_shader->SetTex(var.first, var.second->TextureID);
	}

	void Material::PresetUniforms()
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

}
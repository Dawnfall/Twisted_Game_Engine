#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"

#include <glad/glad.h>

namespace Twisted::Render
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
				
			case ShaderVarType::Sampler2D:
				m_shaderTexVarToIdMap[uniVar.Name] = uniVar.UniformID;
				break;
			case ShaderVarType::Bool:
				m_bools[uniVar.Name] = MaterialValue<bool>{static_cast<int>(uniVar.UniformID) };
				break;
			case ShaderVarType::Float:
				m_floats[uniVar.Name] = MaterialValue<float>{ static_cast<int>(uniVar.UniformID) };
				break;
			case ShaderVarType::Int:
				m_ints[uniVar.Name] = MaterialValue<int>{ static_cast<int>(uniVar.UniformID) };
				break;
			case ShaderVarType::Vec2:
				m_vec2s[uniVar.Name] = MaterialValue<Vec2f>{ static_cast<int>(uniVar.UniformID) };
				break;
			case ShaderVarType::Vec3:
				m_vec3s[uniVar.Name] = MaterialValue<Vec3f>{ static_cast<int>(uniVar.UniformID) };
				break;
			case ShaderVarType::Vec4:
				m_vec4s[uniVar.Name] = MaterialValue<Vec4f>{ static_cast<int>(uniVar.UniformID) };
				break;
			case ShaderVarType::Mat4:
				m_mats[uniVar.Name] = MaterialValue<Mat4x4f>{ static_cast<int>(uniVar.UniformID) };
				break;
			default:
				//DAWN_INFO("Unsupported shader uniform var type");
				break;
			}
		}
	}

}
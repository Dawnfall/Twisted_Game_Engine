#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Shader.h"

#include <variant>
#include <algorithm>
#include <type_traits>

namespace Twisted
{
	void Material::SetShader(Shader* shader)
	{
		if (shader == m_shader)
			return;
		m_shader = shader;

		m_values.clear();
		m_nameToIndex.clear();

		const auto& reflection = m_shader->GetReflection();
		const auto& values = reflection.values;
		for (size_t i = 0; i < values.size(); ++i)
		{
			const auto& shaderReflectedVal = values[i];

			size_t reflIdx = i;
			if (shaderReflectedVal.type == EShaderValueType::TEX_2D)
			{
				// textures are indexed separately in m_bindings.textures,
				// so we need the position in texIndices, not values
				auto it = std::find(reflection.texIndices.begin(), reflection.texIndices.end(), i);
				reflIdx = std::distance(reflection.texIndices.begin(), it);
			}

			m_values.emplace_back(MaterialValue{ GetDefaultValue(shaderReflectedVal.type), shaderReflectedVal.name, reflIdx });
			m_nameToIndex[shaderReflectedVal.name] = m_values.size() - 1;
		}
	}

	bool Material::Clear(const std::string& name)
	{
		auto it = m_nameToIndex.find(name);
		if (it != m_nameToIndex.end())
		{
			resetShaderValue(m_values.at(it->second).val);
			return true;
		}
		return false;
	}

	void Material::ApplyUniforms()
	{
		Shader* shader = this->GetShader();
		if (!shader)
			return;

		for (const auto& uniVar : m_values)
		{
			std::visit([shader,&uniVar](auto&& val) {
				using T = std::decay_t<decltype(val)>;

				if constexpr (std::is_same_v<T, bool>) {
					shader->SetBool(val, uniVar.reflectionIndex);
				}
				else if constexpr (std::is_same_v<T, float>) {
					shader->SetFloat(val, uniVar.reflectionIndex);
				}
				else if constexpr (std::is_same_v<T, int>) {
					shader->SetInt(val, uniVar.reflectionIndex);
				}
				else if constexpr (std::is_same_v<T, Vec3f>) {
					shader->SetVec3(val, uniVar.reflectionIndex);
				}
				else if constexpr (std::is_same_v<T, Vec4f>) {
					shader->SetVec4(val, uniVar.reflectionIndex);
				}
				else if constexpr (std::is_same_v<T, Mat4x4f>) {
					shader->SetMat4(val, uniVar.reflectionIndex);
				}
				else if constexpr (std::is_same_v<T, TextureValue>) {
					if (const Texture* t = val.tex.get())
						shader->SetTexture(t, uniVar.reflectionIndex);
				}
				else {
					TWISTED_ERROR("Unsupported material uniform type to assign to shader!");
				}
				}, uniVar.val);
		}
	}

}
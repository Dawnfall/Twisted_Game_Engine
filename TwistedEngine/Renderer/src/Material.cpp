#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "VulkanContext.h"

#include <variant>
#include <algorithm>
#include <type_traits>

namespace Twisted
{
	void Material::AllocateTextureDescSets()
	{
		FreeTextureDescSets();
		if (!m_shader || m_shader->TextureCount == 0) return;

		auto& ctx = VK::VulkanContext::Get();
		if (ctx.Device == VK_NULL_HANDLE) return;

		VkDescriptorPoolSize poolSize{};
		poolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize.descriptorCount = m_shader->TextureCount * VK::FramesInFlight;

		VkDescriptorPoolCreateInfo poolCI{};
		poolCI.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCI.poolSizeCount = 1;
		poolCI.pPoolSizes    = &poolSize;
		poolCI.maxSets       = VK::FramesInFlight;

		if (vkCreateDescriptorPool(ctx.Device, &poolCI, nullptr, &m_textureDescPool) != VK_SUCCESS)
			throw std::runtime_error("[Vulkan] Failed to create material texture descriptor pool");

		std::vector<VkDescriptorSetLayout> layouts(VK::FramesInFlight, m_shader->TextureDescLayout);
		TextureDescSets.resize(VK::FramesInFlight);

		VkDescriptorSetAllocateInfo ai{};
		ai.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		ai.descriptorPool     = m_textureDescPool;
		ai.descriptorSetCount = VK::FramesInFlight;
		ai.pSetLayouts        = layouts.data();

		if (vkAllocateDescriptorSets(ctx.Device, &ai, TextureDescSets.data()) != VK_SUCCESS)
			throw std::runtime_error("[Vulkan] Failed to allocate material texture descriptor sets");
	}

	void Material::FreeTextureDescSets()
	{
		auto& ctx = VK::VulkanContext::Get();
		if (ctx.Device == VK_NULL_HANDLE) return;

		if (m_textureDescPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(ctx.Device, m_textureDescPool, nullptr);
			m_textureDescPool = VK_NULL_HANDLE;
		}
		TextureDescSets.clear();
	}

	void Material::OnDestroy()
	{
		FreeTextureDescSets();
	}

	Material::~Material()
	{
		FreeTextureDescSets();
	}

	static Texture* GetDefaultWhiteTexture()
	{
		static Texture* s_white = nullptr;
		if (!s_white)
		{
			static unsigned char pixel[4] = { 255, 255, 255, 255 };
			s_white = TObject::Create<Texture>("__default_white");
			s_white->SetData(TextureData{ pixel, 1, 1, 4 }, TextureParams{});
		}
		return s_white;
	}

	static ShaderPropertyType ToEditorType(EShaderValueType t)
	{
		switch (t)
		{
		case EShaderValueType::FLOAT:  return ShaderPropertyType::Float;
		case EShaderValueType::DOUBLE: return ShaderPropertyType::Double;
		case EShaderValueType::INT:    return ShaderPropertyType::Int;
		case EShaderValueType::VEC2_F: return ShaderPropertyType::Vec2;
		case EShaderValueType::VEC3_F: return ShaderPropertyType::Vec3;
		case EShaderValueType::VEC4_F: return ShaderPropertyType::Vec4;
		case EShaderValueType::TEX_2D: return ShaderPropertyType::Texture;
		default:                       return ShaderPropertyType::Float;
		}
	}

	static ShaderValueType DefaultForPropType(ShaderPropertyType t)
	{
		switch (t)
		{
		case ShaderPropertyType::Float:   return float{};
		case ShaderPropertyType::Double:  return double{};
		case ShaderPropertyType::Int:     return int{};
		case ShaderPropertyType::Vec2:    return Vec2f{};
		case ShaderPropertyType::Vec3:    return Vec3f{};
		case ShaderPropertyType::Vec4:    return Vec4f{};
		case ShaderPropertyType::Color:   return Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
		case ShaderPropertyType::Texture: return TextureValue{};
		default:                          return float{};
		}
	}

	void Material::SetShader(Shader* shader)
	{
		if (shader == m_shader)
			return;
		m_shader = shader;

		m_values.clear();
		m_nameToIndex.clear();
		AllocateTextureDescSets();

		const auto& gpuRefl  = m_shader->GetReflection();
		const auto& fileRefl = m_shader->GetShaderFileReflection();

		if (!fileRefl.properties.empty())
		{
			// REF block drives what's exposed — only those uniforms are editable
			for (const auto& prop : fileRefl.properties)
			{
				bool isTexture = (prop.type == ShaderPropertyType::Texture);

				// Find the uniform in the GPU reflection
				size_t reflIdx = SIZE_MAX;
				if (isTexture)
				{
					for (size_t i = 0; i < gpuRefl.texIndices.size(); ++i)
					{
						if (gpuRefl.values[gpuRefl.texIndices[i]].name == prop.shaderName)
						{
							reflIdx = i;
							break;
						}
					}
				}
				else
				{
					for (size_t i = 0; i < gpuRefl.values.size(); ++i)
					{
						if (gpuRefl.values[i].name == prop.shaderName)
						{
							reflIdx = i;
							break;
						}
					}
				}

				if (reflIdx == SIZE_MAX)
					continue; // uniform not present in compiled shader — skip

				m_values.emplace_back(DefaultForPropType(prop.type), prop.shaderName, prop.editorName, prop.type, reflIdx);
				m_nameToIndex[prop.shaderName] = m_values.size() - 1;
			}
		}
		else
		{
			// No REF block — fall back to exposing all GPU-reflected uniforms
			for (size_t i = 0; i < gpuRefl.values.size(); ++i)
			{
				const auto& v = gpuRefl.values[i];

				size_t reflIdx = i;
				if (v.type == EShaderValueType::TEX_2D)
				{
					auto it = std::find(gpuRefl.texIndices.begin(), gpuRefl.texIndices.end(), i);
					reflIdx = static_cast<size_t>(std::distance(gpuRefl.texIndices.begin(), it));
				}

				m_values.emplace_back(GetDefaultValue(v.type), v.name, v.name, ToEditorType(v.type), reflIdx);
				m_nameToIndex[v.name] = m_values.size() - 1;
			}
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
		Shader* shader = GetShader();
		if (!shader)
			return;

		for (const auto& uniVar : m_values)
		{
			std::visit([this, shader, &uniVar](auto&& val) {
				using T = std::decay_t<decltype(val)>;

				if constexpr (std::is_same_v<T, bool>)
					shader->SetBool(val, uniVar.reflectionIndex);
				else if constexpr (std::is_same_v<T, float>)
					shader->SetFloat(val, uniVar.reflectionIndex);
				else if constexpr (std::is_same_v<T, int>)
					shader->SetInt(val, uniVar.reflectionIndex);
				else if constexpr (std::is_same_v<T, Vec3f>)
					shader->SetVec3(val, uniVar.reflectionIndex);
				else if constexpr (std::is_same_v<T, Vec4f>)
					PushColor = val;
				else if constexpr (std::is_same_v<T, Mat4x4f>)
					shader->SetMat4(val, uniVar.reflectionIndex);
				else if constexpr (std::is_same_v<T, TextureValue>) {
					const Texture* t = val.tex.get();
					if (!t) t = GetDefaultWhiteTexture();
					shader->SetTexture(t, uniVar.reflectionIndex, TextureDescSets.data());
				}
				else
					TWISTED_ERROR("Unsupported material uniform type to assign to shader!");
			}, uniVar.val);
		}
	}

}

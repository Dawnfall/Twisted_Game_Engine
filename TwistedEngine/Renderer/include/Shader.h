#pragma once
#include "AppCore.h"
#include "Application/TObject.h"
#include "Utils/GlmUtils.h"
#include "Data/ShaderData.h"
#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace Twisted
{
	class TWISTED_API Shader : public TObject
	{
	public:
		Shader(const std::string& name);
		~Shader() override;

		void Clear();
		void SetData(const ShaderData& data);
		void OnDestroy() override;

		void SetBool(bool val, size_t reflectionIndex);
		void SetInt(int val, size_t reflectionIndex);
		void SetFloat(float val, size_t reflectionIndex);
		void SetVec3(const Vec3f& val, size_t reflectionIndex);
		void SetVec4(const Vec4f& val, size_t reflectionIndex);
		void SetMat4(const Mat4x4f& val, size_t reflectionIndex);
		void SetTexture(const Texture* tex, size_t reflectionIndex, VkDescriptorSet* sets);

		const ShaderReflection& GetReflection()           const { return m_reflection; }
		const ::SPReflection&   GetShaderFileReflection() const { return m_fileReflection; }

		bool IsValid() const { return Pipeline != VK_NULL_HANDLE; }
		bool UsesBlock(uint32_t bindingPoint) const;

		VkPipeline            Pipeline          = VK_NULL_HANDLE;
		VkPipelineLayout      PipelineLayout    = VK_NULL_HANDLE;
		VkDescriptorSetLayout TextureDescLayout = VK_NULL_HANDLE;
		uint32_t              TextureCount      = 0;
		Vec4f                 PushColor         = { 1.0f, 1.0f, 1.0f, 1.0f };

	private:
		ShaderReflection m_reflection;
		::SPReflection   m_fileReflection;
	};

}



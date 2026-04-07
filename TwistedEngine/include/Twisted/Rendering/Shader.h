#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Data/ShaderData.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace Twisted
{
	struct ShaderBackend;
	class TWISTED_API Shader :public TObject
	{
	public:
		Shader(const std::string& name);
		~Shader();

		void Clear();
		void SetData(const ShaderData& data);
		void OnDestroy()override;

		void SetBool(bool val,size_t reflectionIndex);
		void SetInt(int val, size_t reflectionIndex);
		void SetFloat(float val, size_t reflectionIndex);
		void SetVec3(const Vec3f& val, size_t reflectionIndex);
		void SetVec4(const Vec4f& val, size_t reflectionIndex);
		void SetMat4(const Mat4x4f& val, size_t reflectionIndex);
		void SetTexture(const Texture* tex, size_t reflectionIndex);

		//void ApplyUniforms(const std::vector<ActiveValueUniform>& activeUniforms);
		const ShaderReflection& GetReflection()const;

		ShaderBackend* GetBackend() { return m_backend.get(); }
	private:
		URef<ShaderBackend> m_backend = nullptr;
	};

}



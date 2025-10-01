#pragma once
#include "AppCore.h"
#include "Twisted/RegisterLayer/BaseObject.h"
#include "Utils/GlmUtils.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace Twisted
{
	class Material;

	enum class TWISTED_API ShaderVarType : uint8_t
	{
		Float, Vec2, Vec3, Vec4, Mat4, Int, Bool, Sampler2D, Unknown
	};

	struct TWISTED_API ShaderUniformVar
	{
		std::string Name = "";
		ShaderVarType Type = ShaderVarType::Unknown;
		int UniformID = 0;
	};

	struct TWISTED_API ShaderTextureVar
	{
		std::string Name = "";
		ShaderVarType Type = ShaderVarType::Unknown;
		int TextureUnit = 0;
		int UniformID = 0;
	};

	struct ShaderData
	{
		std::string VertShader;
		std::string FragShader;
		std::string GeoShader;
	};

	template<typename T>
	struct TWISTED_API ShaderValue
	{
		int ID = -1;
		T value = T();
	};

	class TWISTED_API Shader :public BaseObject
	{
	public:

		Shader(ObjectID id);
		Shader(ObjectID id, const ShaderData& shaderData);
		~Shader();

		bool IsValid()const { return m_isValid; }

		void Bind()const;
		void UnBind()const;

		void SetVar(int locationID, bool value)const;
		void SetVar(int locationID, int value)const;
		void SetVar(int locationID, float value)const;
		void SetVar(int locationID, const Vec2f& value)const;
		void SetVar(int locationID, const Vec3f& value)const;
		void SetVar(int locationID, const Vec4f& value)const;
		void SetVar(int locationID, const Mat4x4f& value)const;
		void SetTex(int locationID, unsigned int texID)const;

		//void SetTexture(const std::string& shadervarName, Texture* texture)
		//{
		//	int locationID = m_shaderTexVarToIdMap[shadervarName];
		//	m_textures[locationID] = texture;
		//}

		//const std::vector<ShaderTextureVar>& GetTextures() { return m_textures; }

		unsigned int GetProgramID()const { return m_programID; }
		void ApplyUniforms(Material* material)const;

	private:
		void Compile(const ShaderData& shaderData);
		void DetectUniformVars();

		bool m_isValid = false;
		unsigned int m_programID = 0;

		std::vector<ShaderUniformVar> m_uniforms;
		std::vector<ShaderTextureVar> m_textures;

		//std::vector<ShaderTextureVar> m_textures;
	};
}


#pragma once
#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Data/ShaderData.h"

#include <string>
#include <vector>

namespace Twisted::Render
{
	enum class TWISTED_API ShaderVarType : uint8_t
	{
		Float, Vec2, Vec3, Vec4, Mat4, Int, Bool, Sampler2D, Unknown
	};

	struct TWISTED_API ShaderUniformVar
	{
		std::string Name;
		ShaderVarType Type;
		unsigned int UniformID;
	};

	struct TWISTED_API ShaderTextureVar
	{
		std::string Name;
		ShaderVarType Type;
		unsigned int TextureUnit;
		unsigned int UniformID;
	};

	class TWISTED_API Shader
	{
	public:
		Shader(const ShaderData& data) :
			Data(data)
		{
		}

		bool Compile();
		void UnCompile();

		bool IsValid()const { return ProgramID > 0; }

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

		ShaderData Data;
		unsigned int ProgramID = 0;
		std::vector<ShaderUniformVar> Uniforms;
		std::vector<ShaderTextureVar> Textures;

	private:
		void DetectUniformVars();

	};


}


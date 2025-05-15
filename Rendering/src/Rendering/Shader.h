#pragma once
#include "AppCore.h"
#include "Data/ShaderData.h"
#include "Utils/GlmUtils.h"

#include <glad/glad.h>
#include <string>
#include <vector>

namespace Twisted
{
	struct ShaderUniformVar
	{
		std::string Name;
		GLenum Type;
		GLint UniformID;
	};

	struct ShaderTextureVar
	{
		std::string Name;
		GLenum Type;
		GLint UniformID;
		GLint TextureUnit;
	};

	class Shader
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

		void SetVar(GLint locationID, bool value)const;
		void SetVar(GLint locationID, int value)const;
		void SetVar(GLint locationID, float value)const;
		void SetVar(GLint locationID, const Vec2f& value)const;
		void SetVar(GLint locationID, const Vec3f& value)const;
		void SetVar(GLint locationID, const Vec4f& value)const;
		void SetVar(GLint locationID, const Mat4x4f& value)const;
		void SetTex(GLint locationID, GLuint texID)const;

		ShaderData Data;
		GLuint ProgramID = 0;

		std::vector<ShaderUniformVar> Uniforms;
		std::vector<ShaderTextureVar> Textures;

	private:
		void DetectUniformVars();

	};
}


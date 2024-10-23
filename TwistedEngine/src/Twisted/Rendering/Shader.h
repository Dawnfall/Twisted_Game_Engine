#pragma once

#include "pch.h"
#include "Twisted/Resources/ShaderData.h"

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
		Shader(ShaderData data, GLuint programID,const std::vector<ShaderUniformVar>& uniforms) :
			Data(data),
			ProgramID(programID),
			Uniforms(uniforms)
		{}

		bool IsValid() { return ProgramID > 0; }

		GLuint ProgramID = 0;
		ShaderData Data;
		std::vector<ShaderUniformVar> Uniforms;
		std::vector<ShaderTextureVar> Textures;
	};
}


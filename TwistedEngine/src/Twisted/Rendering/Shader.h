#pragma once

#include "twistedpch.h"
#include "AppCore.h"
#include "Assets/ShaderData.h"

namespace Twisted
{
	struct TWISTED_API ShaderUniformVar
	{
		std::string Name;
		GLenum Type;
		GLint UniformID;
	};

	struct TWISTED_API ShaderTextureVar
	{
		std::string Name;
		GLenum Type;
		GLint UniformID;
		GLint TextureUnit;
	};

	class TWISTED_API Shader
	{
	public:
		Shader(std::shared_ptr<ShaderData> data, GLuint programID, const std::vector<ShaderUniformVar>& uniforms) :
			Data(data),
			ProgramID(programID),
			Uniforms(uniforms)
		{}

		bool IsValid() { return ProgramID > 0; }

		std::shared_ptr<ShaderData> Data;
		GLuint ProgramID = 0;
		std::vector<ShaderUniformVar> Uniforms;
		std::vector<ShaderTextureVar> Textures;
	};
}


#pragma once

#include "Twisted/Resources/ShaderData.h"

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

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
		Shader(const std::shared_ptr<ShaderData>& data, GLuint programID,const std::vector<ShaderUniformVar>& uniforms) :
			Data(data),
			ProgramID(programID),
			Uniforms(uniforms)
		{}

		bool IsValid() { return ProgramID > 0; }

		GLuint ProgramID = 0;
		std::shared_ptr<ShaderData> Data;
		std::vector<ShaderUniformVar> Uniforms;
		std::vector<ShaderTextureVar> Textures;
	};
}


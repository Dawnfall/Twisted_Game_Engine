#include "renderingpch.h"
#include "Shader.h"
#include "Utils/GlmUtils.h"
#include "Rendering/OpenGL/Render_OpenGL.h"

namespace Twisted
{
	bool Shader::Compile()
	{
		ProgramID = RenderAPI::CreateShaderProgram(Data.VertShaderCode, Data.FragShaderCode);
		if (ProgramID == 0)
			return false;

		glUseProgram(ProgramID);
		TWISTED_INFO("Shader compile success; shader: " + Data.Name);

		DetectUniformVars();
	}

	void Shader::UnCompile()
	{
		glDeleteShader(ProgramID);
		ProgramID = 0;
	}

	void Shader::DetectUniformVars()
	{
		GLint count = 1;
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(ProgramID, GL_ACTIVE_UNIFORMS, &count);
		GLint textureUnit = 0;
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(ProgramID, (GLuint)i, bufSize, &length, &size, &type, name);
			GLint uniformID = glGetUniformLocation(ProgramID, name);

			if (type == GL_SAMPLER_2D)
			{
				glUniform1i(uniformID, textureUnit);
				Textures.emplace_back(name, type, uniformID, textureUnit++);
			}
			else
				Uniforms.emplace_back(name, type, uniformID);
		}
	}

	void Shader::SetVar(GLint locationID, bool value) const { glUniform1i(locationID, static_cast<int>(value)); }
	void Shader::SetVar(GLint locationID, int value) const { glUniform1i(locationID, value); }
	void Shader::SetVar(GLint locationID, float value) const { glUniform1f(locationID, value); }
	void Shader::SetVar(GLint locationID, const Vec2f& value) const { glUniform2f(locationID, value[0], value[1]); }
	void Shader::SetVar(GLint locationID, const Vec3f& value) const { glUniform3f(locationID, value[0], value[1], value[2]); }
	void Shader::SetVar(GLint locationID, const Vec4f& value) const { glUniform4f(locationID, value[0], value[1], value[2], value[3]); }
	void Shader::SetVar(GLint locationID, const Mat4x4f& value) const { glUniformMatrix4fv(locationID, 1, GL_FALSE, &value[0][0]); }
	void Shader::SetTex(GLint locationID, GLuint texID)const
	{
		glActiveTexture(GL_TEXTURE0 + locationID);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	void Shader::Bind()const
	{
		glUseProgram(ProgramID);
	}
	void Shader::UnBind()const
	{
		glUseProgram(0);
	}
}

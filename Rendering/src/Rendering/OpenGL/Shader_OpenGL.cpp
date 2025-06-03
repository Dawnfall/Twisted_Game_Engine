#include "Rendering/Shader.h"
#include <glad/glad.h>

namespace Twisted::Render
{
	ShaderVarType FromGLShaderType(GLenum glShaderVarType)
	{
		switch (glShaderVarType)
		{
		case GL_INT:
			return ShaderVarType::Int;
		case GL_FLOAT:
			return ShaderVarType::Float;
		case GL_BOOL:
			return ShaderVarType::Bool;
		case GL_FLOAT_VEC2:
			return ShaderVarType::Vec2;
		case GL_FLOAT_VEC3:
			return ShaderVarType::Vec3;
		case GL_FLOAT_VEC4:
			return ShaderVarType::Vec4;
		case GL_FLOAT_MAT4:
			return ShaderVarType::Mat4;
		default:
			TWISTED_WARN("Unknown shader type");
			return ShaderVarType::Unknown;
		}
	}

	GLenum ToGlShaderType(ShaderVarType shaderVarType)
	{
		switch (shaderVarType)
		{
		case ShaderVarType::Int:
			return GL_INT;
		case ShaderVarType::Float:
			return GL_FLOAT;
		case ShaderVarType::Bool:
			return GL_BOOL;
		case ShaderVarType::Vec2:
			return GL_FLOAT_VEC2;
		case ShaderVarType::Vec3:
			return GL_FLOAT_VEC3;
		case ShaderVarType::Vec4:
			return GL_FLOAT_VEC4;
		case ShaderVarType::Mat4:
			return GL_FLOAT_MAT4;
		default:
			TWISTED_WARN("Unknown shader type");
			return 0;
		}
	}


	GLuint CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode)
	{
		GLuint shaderID = glCreateShader(shaderType);

		if (shaderID == 0)
		{
			TWISTED_ERROR("Error creating shader");
			return 0;
		}

		glShaderSource(shaderID, 1, &shaderCode, NULL);
		glCompileShader(shaderID);

		int success;
		char infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			TWISTED_ERROR(std::string("Error: Failed to compile ") + shaderName + " shader! " + infoLog);
			return 0;
		}
		return shaderID;
	}
	GLuint CompileProgram(GLuint vertexID, GLuint fragmentID)
	{
		GLuint shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, vertexID);
		glAttachShader(shaderProgramID, fragmentID);
		glLinkProgram(shaderProgramID);

		int success;
		char infoLog[512];

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (success != GL_TRUE)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			TWISTED_ERROR(std::string("Error: Failed to compile shader program!") + infoLog);
			return 0;
		}
		return shaderProgramID;
	}

	unsigned int CreateShaderProgram(const std::string& vertexShaderCode, const std::string& fragShaderCode)
	{
		GLuint vertexID = 0;
		GLuint fragmentID = 0;

		if (vertexShaderCode != "")
			vertexID = CompileShaderCode(GL_VERTEX_SHADER, "Vertex", vertexShaderCode.c_str());
		if (fragShaderCode != "")
			fragmentID = CompileShaderCode(GL_FRAGMENT_SHADER, "Fragment", fragShaderCode.c_str());
		GLuint programID = CompileProgram(vertexID, fragmentID);

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		if (programID < 1)
		{
			TWISTED_WARN("Shader compile failure; shader: ");
			return 0;
		}
		return programID;
	}

	bool Shader::Compile()
	{
		ProgramID = CreateShaderProgram(Data.VertShaderCode, Data.FragShaderCode);
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
				ShaderTextureVar texVar{ name,FromGLShaderType(type), uniformID, textureUnit++ };
				Textures.emplace_back(texVar);
			}
			else
			{
				ShaderUniformVar uniVar{ name,FromGLShaderType(type),uniformID };
				Uniforms.emplace_back(uniVar);
			}
		}
	}

	void Shader::Bind()const
	{
		glUseProgram(ProgramID);
	}
	void Shader::UnBind()const
	{
		glUseProgram(0);
	}

	void Shader::SetVar(int locationID, bool value) const { glUniform1i(locationID, static_cast<int>(value)); }
	void Shader::SetVar(int locationID, int value) const { glUniform1i(locationID, value); }
	void Shader::SetVar(int locationID, float value) const { glUniform1f(locationID, value); }
	void Shader::SetVar(int locationID, const Vec2f& value) const { glUniform2f(locationID, value[0], value[1]); }
	void Shader::SetVar(int locationID, const Vec3f& value) const { glUniform3f(locationID, value[0], value[1], value[2]); }
	void Shader::SetVar(int locationID, const Vec4f& value) const { glUniform4f(locationID, value[0], value[1], value[2], value[3]); }
	void Shader::SetVar(int locationID, const Mat4x4f& value) const { glUniformMatrix4fv(locationID, 1, GL_FALSE, &value[0][0]); }
	void Shader::SetTex(int locationID, unsigned int texID)const
	{
		glActiveTexture(GL_TEXTURE0 + locationID);
		glBindTexture(GL_TEXTURE_2D, texID);
	}
}
#include "Shader.h"
#include "Debug/Logger.h"
#include <glm.hpp>
#include <glad/glad.h>

namespace Dawn
{
	Shader::Shader(std::shared_ptr<ShaderData>& data)
	{
		Data = data;
	}

	void Shader::Bind(const Shader& shader)
	{
		glUseProgram(shader.ID);
	}

	void Shader::UnCompileShader(Shader& shader)
	{
		glDeleteShader(shader.ID);
		shader.ID = -1;
		shader.isValid = false;
	}

	void Shader::CompileShader(Shader& shader)
	{
		unsigned int vertexID, fragmentID;

		if (shader.Data->VertShaderCode != "")
			unsigned int vertexShaderID = Shader::CompileShader(GL_VERTEX_SHADER, "Vertex", vertexID, shader.Data->VertShaderCode.c_str());
		if (shader.Data->FragShaderCode != "")
			unsigned int fragmentShaderID = Shader::CompileShader(GL_FRAGMENT_SHADER, "Fragment", fragmentID, shader.Data->FragShaderCode.c_str());
		shader.ID = Shader::CompileProgram(vertexID, fragmentID);

		if (shader.ID > 0)
		{
			DAWN_INFO("Shader compile success; shader: " + shader.Data->Name);
		}

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
	}

	unsigned int Shader::CompileShader(GLenum shaderType, const char* shaderName, unsigned int& shaderID, const char* shaderCode)
	{
		shaderID = glCreateShader(shaderType);

		if (shaderID == 0)
		{
			DAWN_ERROR("Error creating shader");
			return 0;
		}

		glShaderSource(shaderID, 1, &shaderCode, nullptr);
		glCompileShader(shaderID);

		int success;
		char infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
			DAWN_ERROR(std::string("Error: Failed to compile ") + shaderName + " shader! " + infoLog);
			return 0;
		}
		return shaderID;
	}
	unsigned int Shader::CompileProgram(GLenum vertexID, GLenum fragmentID)
	{
		unsigned int shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, vertexID);
		glAttachShader(shaderProgramID, fragmentID);
		glLinkProgram(shaderProgramID);

		int success;
		char infoLog[512];

		glGetProgramiv(shaderProgramID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
			DAWN_ERROR(std::string("Error: Failed to compile shader program!") + infoLog);
			return false;
		}
		return true;
	}

	void Shader::setBool(unsigned int shaderID, const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), static_cast<int>(value));
	}
	void Shader::setInt(unsigned int shaderID, const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
	}
	void Shader::setFloat(unsigned int shaderID, const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
	}
	void Shader::setFloat2(unsigned int shaderID, const std::string& name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(shaderID, name.c_str()), value[0], value[1]);
	}
	void Shader::setFloat3(unsigned int shaderID, const std::string& name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), value[0], value[1], value[2]);
	}
	void Shader::setFloat4(unsigned int shaderID, const std::string& name, const glm::vec4& value)
	{
		glUniform4f(glGetUniformLocation(shaderID, name.c_str()), value[0], value[1], value[2], value[3]);
	}
	void Shader::setMat4(unsigned int shaderID, const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}


}
#include "Shader.h"

#include <glm.hpp>
#include <glad/glad.h>

namespace Dawn
{
	Shader::Shader(const char* vertexShaderCode, const char* fragmentShaderCode)
	{
		unsigned int vertexID, fragmentID;

		CompileShader(GL_VERTEX_SHADER, "Vertex", vertexID, vertexShaderCode);
		CompileShader(GL_FRAGMENT_SHADER, "Fragment", fragmentID, fragmentShaderCode);
		CompileProgram(vertexID, fragmentID);

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
	}

	Shader::~Shader()
	{
		glDeleteShader(m_ID);
	}

	void Shader::Bind()
	{
		glUseProgram(m_ID);
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
	}
	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void Shader::setFloat2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value[0],value[1]);
	}
	void Shader::setFloat3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2]);
	}
	void Shader::setFloat4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2],value[3]);
	}
	void Shader::mat4(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()),1,GL_FALSE,&value[0][0]);
	}

	bool Shader::CompileShader(GLenum shaderType, const char* shaderName, unsigned int& shaderID, const char* shaderCode)
	{
		shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &shaderCode, nullptr);
		glCompileShader(shaderID);

		int success;
		char infoLog[512];

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
			std::cout << "Error: Failed to compile " << shaderName << " shader!\n" << infoLog << std::endl;
			return false;
		}
		return true;
	}
	bool Shader::CompileProgram(GLenum vertexID, GLenum fragmentID)
	{
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexID);
		glAttachShader(m_ID, fragmentID);
		glLinkProgram(m_ID);

		int success;
		char infoLog[512];

		glGetProgramiv(m_ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
			std::cout << "Error: Failed to compile shader program!\n" << infoLog << std::endl;
			return false;
		}
		return true;
	}
}
#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>

namespace Dawn
{

	class Shader
	{
	public:
		Shader(const char* vertexShaderCode, const char* fragmentShaderCode)
		{
			unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexID, 1, &vertexShaderCode, nullptr);
			glCompileShader(vertexID);
			checkCompileErrors(vertexID, "VERTEX");

			unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentID, 1, &fragmentShaderCode, nullptr);
			glCompileShader(fragmentID);
			checkCompileErrors(fragmentID, "FRAGMENT");

			m_ID = glCreateProgram();
			glAttachShader(m_ID, vertexID);
			glAttachShader(m_ID, fragmentID);
			glLinkProgram(m_ID);
			checkCompileErrors(m_ID, "PROGRAM");

			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);
		}

		~Shader()
		{
			glDeleteShader(m_ID);
		}

		void Bind()
		{
			glUseProgram(m_ID);
		}

		// Utility uniform functions
		void setBool(const std::string& name, bool value) const {
			glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
		}
		void setInt(const std::string& name, int value) const {
			glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
		}
		void setFloat(const std::string& name, float value) const {
			glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
		}
	private:
		unsigned int m_ID = 0;

		void checkCompileErrors(unsigned int shader, const std::string& type) {
			int success;
			char infoLog[512];
			if (type != "PROGRAM") {
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(shader, 512, nullptr, infoLog);
					std::cout << "Error: Failed to compile " << type << " shader!\n" << infoLog << std::endl;
				}
			}
			else {
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shader, 512, nullptr, infoLog);
					std::cout << "Error: Failed to link shader program!\n" << infoLog << std::endl;
				}
			}
		}

	};
}


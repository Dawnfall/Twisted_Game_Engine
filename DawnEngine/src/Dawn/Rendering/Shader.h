#pragma once

#include <glad/glad.h>
#include <glm.hpp>

#include <iostream>
#include <string>

namespace Dawn
{

	class Shader
	{
	public:
		Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
		~Shader();

		void Bind();

		// Utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setFloat2(const std::string& name, const glm::vec2& value)const;
		void setFloat3(const std::string& name, const glm::vec3& value)const;
		void setFloat4(const std::string& name, const glm::vec4& value)const;
		void mat4(const std::string& name, const glm::mat4& value)const;
	private:
		unsigned int m_ID = 0;

		bool CompileShader(GLenum shaderType, const char* shaderName, unsigned int& shaderID, const char* shaderCode);
		bool CompileProgram(GLenum vertexID, GLenum fragmentID);

	};
}


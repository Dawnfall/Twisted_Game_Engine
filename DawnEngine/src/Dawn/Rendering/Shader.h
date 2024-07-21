#pragma once

#include "Dawn/Resources/ShaderData.h"

#include <glad/glad.h>
#include <glm.hpp>

#include <iostream>
#include <string>

namespace Dawn
{
	class Shader
	{
	public:
		Shader(std::shared_ptr<ShaderData>& data);

		std::shared_ptr<ShaderData> Data;
		bool IsValid() {}

		unsigned int ID = 0;
	private:
		bool isValid = false;

	public:
		static void Bind(const Shader& shader);

		static void CompileShader(Shader& shader);
		static void UnCompileShader(Shader& shader);

		static unsigned int CompileShader(GLenum shaderType, const char* shaderName, unsigned int& shaderID, const char* shaderCode);
		static unsigned int CompileProgram(GLenum vertexID, GLenum fragmentID);

		// Utility uniform functions
		static void setBool(unsigned int shaderID, const std::string& name, bool value);
		static void setInt(unsigned int shaderID, const std::string& name, int value);
		static void setFloat(unsigned int shaderID, const std::string& name, float value);
		static void setFloat2(unsigned int shaderID, const std::string& name, const glm::vec2& value);
		static void setFloat3(unsigned int shaderID, const std::string& name, const glm::vec3& value);
		static void setFloat4(unsigned int shaderID, const std::string& name, const glm::vec4& value);
		static void setMat4(unsigned int shaderID, const std::string& name, const glm::mat4& value);
	};
}


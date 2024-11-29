#include "twistedpch.h"

#include "Shader_OpenGL.h"

namespace Twisted::ShaderAPI
{
	void UnCompileShader(Shader& shader)
	{
		glDeleteShader(shader.ProgramID);
		shader.ProgramID = -1;
	}

	SRef<Shader> CreateShader(SRef<ShaderData> shaderData)
	{
		GLuint vertexID = 0;
		GLuint fragmentID = 0;

		if (shaderData->VertShaderCode != "")
			vertexID = ShaderAPI::CompileShader(GL_VERTEX_SHADER, "Vertex", shaderData->VertShaderCode.c_str());
		if (shaderData->FragShaderCode != "")
			fragmentID = ShaderAPI::CompileShader(GL_FRAGMENT_SHADER, "Fragment", shaderData->FragShaderCode.c_str());
		GLuint programID = ShaderAPI::CompileProgram(vertexID, fragmentID);

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		if (programID < 1)
		{
			TWISTED_WARN("Shader compile failure; shader: " + shaderData->Name);
			return nullptr;
		}

		glUseProgram(programID);
		std::vector<ShaderUniformVar> uniforms = DetectUniformVars(programID);

		TWISTED_INFO("Shader compile success; shader: " + shaderData->Name);
		return std::make_shared<Shader>(shaderData, programID, uniforms);
	}

	GLuint CompileShader(GLenum shaderType, const char* shaderName, const char* shaderCode)
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

	std::vector<ShaderUniformVar> DetectUniformVars(GLuint programID)
	{
		std::vector<ShaderUniformVar> uniforms;
		std::vector<ShaderTextureVar> textures;

		GLint count = 1;
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);

		printf("Active Uniforms: %d\n", count);

		GLint textureUnit = 0;
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(programID, (GLuint)i, bufSize, &length, &size, &type, name);
			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);

			GLint uniformID = glGetUniformLocation(programID, name);

			if (type == GL_SAMPLER_2D)
			{
				glUniform1i(uniformID, textureUnit);
				textures.emplace_back(name, type, uniformID, textureUnit++);
			}
			else
				uniforms.emplace_back(name, type, uniformID);
		}
		return uniforms;
	}

	void SetUniforms(const SRef<Material>& material)
	{
		for (auto& var : material->m_bools)
			setBool(var.second.ID, var.second.value);
		for (auto& var : material->m_ints)
			setInt(var.second.ID, var.second.value);
		for (auto& var : material->m_floats)
			setFloat(var.second.ID, var.second.value);
		for (auto& var : material->m_vec2s)
			setFloat2(var.second.ID, var.second.value);
		for (auto& var : material->m_vec3s)
			setFloat3(var.second.ID, var.second.value);
		for (auto& var : material->m_vec4s)
			setFloat4(var.second.ID, var.second.value);
		for (auto& var : material->m_mats)
			setMat4(var.second.ID, var.second.value);
		for (auto& var : material->m_textures)
		{
			glActiveTexture(GL_TEXTURE0 + var.first);
			glBindTexture(GL_TEXTURE_2D, var.second->TextureID);
		}
	}

	void setBool(GLint locationID, bool value)
	{
		glUniform1i(locationID, static_cast<int>(value));
	}
	void setInt(GLint locationID, int value)
	{
		glUniform1i(locationID, value);
	}
	void setFloat(GLint locationID, float value)
	{
		glUniform1f(locationID, value);
	}
	void setFloat2(GLint locationID, const Vec2f& value)
	{
		glUniform2f(locationID, value[0], value[1]);
	}
	void setFloat3(GLint locationID, const Vec3f& value)
	{
		glUniform3f(locationID, value[0], value[1], value[2]);
	}
	void setFloat4(GLint locationID, const Vec4f& value)
	{
		glUniform4f(locationID, value[0], value[1], value[2], value[3]);
	}
	void setMat4(GLint locationID, const Mat4x4f& value)
	{
		glUniformMatrix4fv(locationID, 1, GL_FALSE, &value[0][0]);
	}
}
#include "Twisted/Rendering/OpenGL/Shader_OpenGL.h"
#include "Twisted/Rendering/Shader.h"

#include "Debug/Logger.h"

#include <glad/glad.h>
#include <string>
#include <vector>

namespace Twisted
{
	Shader::Shader(const std::string& name) :
		TObject(name)
	{
	}
	void Shader::OnDestroy()
	{
		Shader_GL::Clear(*this);
	}
}

namespace Twisted::Shader_GL
{
	ShaderVarType FromGLShaderType(GLenum glShaderVarType)
	{
		switch (glShaderVarType)
		{
		case GL_UNSIGNED_INT:
			return ShaderVarType::UNSIGNED_INT;
		case GL_INT:
			return ShaderVarType::INT;
		case GL_FLOAT:
			return ShaderVarType::FLOAT;
		case GL_BOOL:
			return ShaderVarType::BOOL;
		case GL_FLOAT_VEC2:
			return ShaderVarType::VEC2_F;
		case GL_FLOAT_VEC3:
			return ShaderVarType::VEC3_F;
		case GL_FLOAT_VEC4:
			return ShaderVarType::VEC4_F;
		case GL_DOUBLE_VEC2:
			return ShaderVarType::VEC2_D;
		case GL_DOUBLE_VEC3:
			return ShaderVarType::VEC3_D;
		case GL_DOUBLE_VEC4:
			return ShaderVarType::VEC4_D;
		case GL_INT_VEC2:
			return ShaderVarType::VEC2_I;
		case GL_INT_VEC3:
			return ShaderVarType::VEC3_I;
		case GL_INT_VEC4:
			return ShaderVarType::VEC4_I;
		case GL_UNSIGNED_INT_VEC2:
			return ShaderVarType::VEC2_U;
		case GL_UNSIGNED_INT_VEC3:
			return ShaderVarType::VEC3_U;
		case GL_UNSIGNED_INT_VEC4:
			return ShaderVarType::VEC4_U;
		case GL_FLOAT_MAT4:
			return ShaderVarType::MAT4x4_F;
		case GL_FLOAT_MAT3:
			return ShaderVarType::MAT3x3_F;
		case GL_FLOAT_MAT2:
			return ShaderVarType::MAT2x2_F;
		case GL_DOUBLE_MAT4:
			return ShaderVarType::MAT4x4_D;
		case GL_DOUBLE_MAT3:
			return ShaderVarType::MAT3x3_D;
		case GL_DOUBLE_MAT2:
			return ShaderVarType::MAT2x2_D;
		case GL_SAMPLER_2D:
			return ShaderVarType::SAMPLER2D;

		default:
			TWISTED_WARN("Unknown shader type");
			assert(false);
			return ShaderVarType::UNKNOWN;
		}
	}

	GLenum ToGlShaderType(ShaderVarType shaderVarType)
	{
		switch (shaderVarType)
		{
		case ShaderVarType::INT:
			return GL_INT;
		case ShaderVarType::FLOAT:
			return GL_FLOAT;
		case ShaderVarType::BOOL:
			return GL_BOOL;
		case ShaderVarType::VEC2_F:
			return GL_FLOAT_VEC2;
		case ShaderVarType::VEC3_F:
			return GL_FLOAT_VEC3;
		case ShaderVarType::VEC4_F:
			return GL_FLOAT_VEC4;
		case ShaderVarType::VEC2_D:
			return GL_DOUBLE_VEC2;
		case ShaderVarType::VEC3_D:
			return GL_DOUBLE_VEC3;
		case ShaderVarType::VEC4_D:
			return GL_DOUBLE_VEC4;
		case ShaderVarType::VEC2_I:
			return GL_INT_VEC2;
		case ShaderVarType::VEC3_I:
			return GL_INT_VEC3;
		case ShaderVarType::VEC4_I:
			return GL_INT_VEC4;
		case ShaderVarType::VEC2_U:
			return GL_UNSIGNED_INT_VEC2;
		case ShaderVarType::VEC3_U:
			return GL_UNSIGNED_INT_VEC3;
		case ShaderVarType::VEC4_U:
			return GL_UNSIGNED_INT_VEC4;
		case ShaderVarType::MAT2x2_F:
			return GL_FLOAT_MAT2;
		case ShaderVarType::MAT3x3_F:
			return GL_FLOAT_MAT3;
		case ShaderVarType::MAT4x4_F:
			return GL_FLOAT_MAT4;
		case ShaderVarType::MAT2x2_D:
			return GL_DOUBLE_MAT2;
		case ShaderVarType::MAT3x3_D:
			return GL_DOUBLE_MAT3;
		case ShaderVarType::MAT4x4_D:
			return GL_DOUBLE_MAT4;
		case ShaderVarType::SAMPLER2D:
			return GL_SAMPLER_2D;
		default:
			TWISTED_WARN("Unknown shader type");
			assert(false);
			return 0;
		}
	}

	bool ValidateShader(GLuint id, const std::string& name, GLenum shaderType)
	{
		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		GLint logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::string infoLog(logLength, '\0');
			glGetShaderInfoLog(id, logLength, NULL, infoLog.data());

			if (success != GL_TRUE)
			{
				TWISTED_ERROR(std::string("Error: Failed to compile shader: ") + std::to_string((unsigned int)shaderType) + " ; " + name + " Log: " + infoLog);
				return false;
			}
			else
			{
				TWISTED_WARN(std::string("Shader compiled with warnings: ") + std::to_string((unsigned int)shaderType) + " ; " + name + "Log: " + infoLog);
			}
		}
		else
		{
			TWISTED_INFO("Shader compile success: " + name);
		}
		return true;
	}

	bool ValidateProgram(GLuint id, const std::string& name)
	{
		GLint success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);

		GLint logLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::string infoLog(logLength, '\0');
			glGetProgramInfoLog(id, logLength, NULL, infoLog.data());

			if (success != GL_TRUE)
			{
				TWISTED_ERROR(std::string("Error: Failed to compile shader program: ") + name + " Log: " + infoLog);
				return false;
			}
			else
			{
				TWISTED_WARN(std::string("Shader program compiled with warnings: ") + name + " Log: " + infoLog);
			}
		}
		else
		{
			TWISTED_INFO("Shader program compile success: " + name);
		}
		return true;
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

		if (!ValidateShader(shaderID, shaderName, GL_COMPILE_STATUS))
		{
			glDeleteShader(shaderID);
			shaderID = 0;
		}
		return shaderID;
	}


	void SetData(Shader& shader, const ShaderData& shaderData)
	{
		if (shaderData.VertShader == "")
		{
			TWISTED_ERROR("Shader compilation failed: missing vertex shader");
			return;
		}
		if (shaderData.FragShader == "")
		{
			TWISTED_ERROR("Shader compilation failed: missing fragment shader");
			return;
		}

		GLuint vertexID = Shader_GL::CompileShaderCode(GL_VERTEX_SHADER, "Vertex", shaderData.VertShader.c_str());
		GLuint fragmentID = Shader_GL::CompileShaderCode(GL_FRAGMENT_SHADER, "Fragment", shaderData.FragShader.c_str());
		GLuint geometryID = 0;
		if (shaderData.GeoShader != "")
			geometryID = Shader_GL::CompileShaderCode(GL_GEOMETRY_SHADER, "Geometry", shaderData.GeoShader.c_str());

		shader.ProgramID = Shader_GL::LinkProgram(vertexID, fragmentID, geometryID);
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		glDeleteShader(geometryID);

		if (shader.IsValid())
		{
			shader.Uniforms = Shader_GL::detectShaderUniformVars(shader.ProgramID);
			shader.UniformBlocks = Shader_GL::detectShaderUniformBlocks(shader.ProgramID);
		}
	}

	void Clear(Shader& shader)
	{
		if (shader.ProgramID == 0)
			return;

		glDeleteProgram(shader.ProgramID);
		shader.ProgramID = 0;
		shader.Uniforms.clear();
	}

	void Bind(const Shader& shader)
	{
		glUseProgram(shader.ProgramID);
	}

	void UnBind()
	{
		glUseProgram(0);
	}


	std::vector<ShaderUniformBlock> detectShaderUniformBlocks(GLuint shaderProgram)
	{
		std::vector<ShaderUniformBlock> uniformBlocks;

		//block uniforms
		GLint blockCount = 0;
		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_BLOCKS, &blockCount);

		GLuint nextBindingPoint = 0;
		for (GLuint i = 0; i < (GLuint)blockCount; ++i)
		{

			char name[256];
			GLsizei length = 0;
			glGetActiveUniformBlockName(shaderProgram, i, sizeof(name), &length, name);

			GLint blockSize = 0;
			glGetActiveUniformBlockiv(shaderProgram, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

			// store block name, size, and index
			ShaderUniformBlock blockInfo;
			blockInfo.Name = std::string(name, length);
			blockInfo.Index = i;
			blockInfo.Size = blockSize;

			// ---------------------------
			// Create UBO buffer
			// ---------------------------
			glGenBuffers(1, &blockInfo.BufferID);
			glBindBuffer(GL_UNIFORM_BUFFER, blockInfo.BufferID);
			glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// ---------------------------
			// Assign binding point
			// ---------------------------
			blockInfo.Binding = nextBindingPoint++;
			glUniformBlockBinding(shaderProgram, blockInfo.Index, blockInfo.Binding);

			// ---------------------------
			// Bind UBO object to binding point
			// ---------------------------
			glBindBufferBase(GL_UNIFORM_BUFFER, blockInfo.Binding, blockInfo.BufferID);

			uniformBlocks.push_back(blockInfo);
		}
		return uniformBlocks;
	}

	std::vector<ShaderUniformVar> detectShaderUniformVars(GLuint shaderProgram)
	{
		std::vector<ShaderUniformVar> uniforms;

		GLint count = 0;
		GLint varSize;
		GLenum varType;

		GLint maxNameLength = 0;
		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
		std::vector<GLchar> nameBuffer(maxNameLength);

		glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &count);
		GLint textureUnit = 0;
		for (int i = 0; i < count; i++)
		{
			GLsizei nameLength;
			glGetActiveUniform(shaderProgram, (GLuint)i, maxNameLength, &nameLength, &varSize, &varType, nameBuffer.data());
			GLint uniformID = glGetUniformLocation(shaderProgram, nameBuffer.data());

			ShaderUniformVar uniformVar;
			uniformVar.Name = nameBuffer.data();
			uniformVar.Type = FromGLShaderType(varType);
			uniformVar.UniformID = uniformID;
			if (varType == GL_SAMPLER_2D)
				uniformVar.TextureUnit = textureUnit++;

			uniforms.emplace_back(uniformVar);
		}
		return uniforms;
	}

	[[nodiscard]] GLuint LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID)
	{
		if (vertexID == 0 || fragmentID == 0)
			return 0;

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexID);
		glAttachShader(program, fragmentID);
		if (geometryID != 0)
			glAttachShader(program, geometryID);

		glLinkProgram(program);
		if (!ValidateProgram(program, "Shader program"))
		{
			glDeleteProgram(program);
			program = 0;
		}
		return program;
	}

	void SetVar(int locationID, bool value) { glUniform1i(locationID, static_cast<int>(value)); }
	void SetVar(int locationID, int value) { glUniform1i(locationID, value); }
	void SetVar(int locationID, float value) { glUniform1f(locationID, value); }
	void SetVar(int locationID, double value) { glUniform1d(locationID, value); }
	void SetVar(int locationID, unsigned int value) { glUniform1ui(locationID, value); }
	void SetVar(int locationID, const Vec4f& value) { glUniform4f(locationID, value[0], value[1], value[2], value[3]); }
	void SetVar(int locationID, const Vec3f& value) { glUniform3f(locationID, value[0], value[1], value[2]); }
	void SetVar(int locationID, const Vec2f& value) { glUniform2f(locationID, value[0], value[1]); }
	void SetVar(int locationID, const Vec4d& value) { glUniform4d(locationID, value[0], value[1], value[2], value[3]); }
	void SetVar(int locationID, const Vec3d& value) { glUniform3d(locationID, value[0], value[1], value[2]); }
	void SetVar(int locationID, const Vec2d& value) { glUniform2d(locationID, value[0], value[1]); }
	void SetVar(int locationID, const Vec4i& value) { glUniform4i(locationID, value[0], value[1], value[2], value[3]); }
	void SetVar(int locationID, const Vec3i& value) { glUniform3i(locationID, value[0], value[1], value[2]); }
	void SetVar(int locationID, const Vec2i& value) { glUniform2i(locationID, value[0], value[1]); }
	void SetVar(int locationID, const Mat4x4f& value) { glUniformMatrix4fv(locationID, 1, GL_FALSE, &value[0][0]); }
	void SetVar(int locationID, const Mat3x3f& value) { glUniformMatrix3fv(locationID, 1, GL_FALSE, &value[0][0]); }
	void SetVar(int locationID, const Mat2x2f& value) { glUniformMatrix2fv(locationID, 1, GL_FALSE, &value[0][0]); }
	void SetVar(int locationID, const Mat4x4d& value) { glUniformMatrix4dv(locationID, 1, GL_FALSE, &value[0][0]); }
	void SetVar(int locationID, const Mat3x3d& value) { glUniformMatrix3dv(locationID, 1, GL_FALSE, &value[0][0]); }
	void SetVar(int locationID, const Mat2x2d& value) { glUniformMatrix2dv(locationID, 1, GL_FALSE, &value[0][0]); }
	void SetTex(int locationID, int unit, unsigned int texID)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texID);
		glUniform1i(locationID, unit);
	}
	void SetBuffer(const std::string& name,const Shader& shader, const void* data)
	{
		for (const auto& block : shader.UniformBlocks)
		{
			if (block.Name == name)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, block.BufferID);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, block.Size, data);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
				return;
			}
		}
	}	
}


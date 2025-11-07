#include "Twisted/Rendering/Shader.h"
#include "Debug/Logger.h"

#include <glad/glad.h>
namespace Twisted
{
	static ShaderVarType FromGLShaderType(GLenum glShaderVarType)
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

	static GLenum ToGlShaderType(ShaderVarType shaderVarType)
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

	static bool ValidateShader(GLuint id, const std::string& name, GLenum shaderType)
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

	static bool ValidateProgram(GLuint id, const std::string& name)
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

	static GLuint CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode)
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

	static GLuint LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID)
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

	void Shader::Init(const ShaderData& shaderData)
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

		GLuint vertexID = CompileShaderCode(GL_VERTEX_SHADER, "Vertex", shaderData.VertShader.c_str());
		GLuint fragmentID = CompileShaderCode(GL_FRAGMENT_SHADER, "Fragment", shaderData.FragShader.c_str());
		GLuint geometryID = 0;
		if (shaderData.GeoShader != "")
			geometryID = CompileShaderCode(GL_GEOMETRY_SHADER, "Geometry", shaderData.GeoShader.c_str());

		m_programID = LinkProgram(vertexID, fragmentID, geometryID);
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		glDeleteShader(geometryID);

		if (IsValid())
			DetectUniformVars();
	}

	void Shader::Clear()
	{
		if (m_programID == 0)
			return;

		glDeleteProgram(m_programID);
		m_programID = 0;
		m_uniforms.clear();
	}

	void Shader::Bind()const
	{
		glUseProgram(m_programID);
	}

	void Shader::UnBind()const
	{
		glUseProgram(0);
	}

	void Shader::DetectUniformVars()
	{
		GLint count = 0;
		GLint varSize;
		GLenum varType;

		GLint maxNameLength = 0;
		glGetProgramiv(m_programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
		std::vector<GLchar> nameBuffer(maxNameLength);

		glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &count);
		GLint textureUnit = 0;
		for (int i = 0; i < count; i++)
		{
			GLsizei nameLength;
			glGetActiveUniform(m_programID, (GLuint)i, maxNameLength, &nameLength, &varSize, &varType, nameBuffer.data());
			GLint uniformID = glGetUniformLocation(m_programID, nameBuffer.data());

			ShaderUniformVar uniformVar;
			uniformVar.Name = nameBuffer.data();
			uniformVar.Type = FromGLShaderType(varType);
			uniformVar.UniformID = uniformID;
			if (varType == GL_SAMPLER_2D)
				uniformVar.TextureUnit = textureUnit++;

			m_uniforms.emplace_back(uniformVar);
		}
	}

	void Shader::SetVar(int locationID, bool value) const { glUniform1i(locationID, static_cast<int>(value)); }
	void Shader::SetVar(int locationID, int value) const { glUniform1i(locationID, value); }
	void Shader::SetVar(int locationID, float value) const { glUniform1f(locationID, value); }
	void Shader::SetVar(int locationID, double value) const { glUniform1d(locationID, value); }
	void Shader::SetVar(int locationID, unsigned int value) const { glUniform1ui(locationID, value); }

	void Shader::SetVar(int locationID, const Vec4f& value) const { glUniform4f(locationID, value[0], value[1], value[2], value[3]); }
	void Shader::SetVar(int locationID, const Vec3f& value) const { glUniform3f(locationID, value[0], value[1], value[2]); }
	void Shader::SetVar(int locationID, const Vec2f& value) const { glUniform2f(locationID, value[0], value[1]); }

	void Shader::SetVar(int locationID, const Vec4d& value) const { glUniform4d(locationID, value[0], value[1], value[2], value[3]); }
	void Shader::SetVar(int locationID, const Vec3d& value) const { glUniform3d(locationID, value[0], value[1], value[2]); }
	void Shader::SetVar(int locationID, const Vec2d& value) const { glUniform2d(locationID, value[0], value[1]); }

	void Shader::SetVar(int locationID, const Vec4i& value) const { glUniform4i(locationID, value[0], value[1], value[2], value[3]); }
	void Shader::SetVar(int locationID, const Vec3i& value) const { glUniform3i(locationID, value[0], value[1], value[2]); }
	void Shader::SetVar(int locationID, const Vec2i& value) const { glUniform2i(locationID, value[0], value[1]); }

	void Shader::SetVar(int locationID, const Mat4x4f& value) const { glUniformMatrix4fv(locationID, 1, GL_FALSE, &value[0][0]); }
	void Shader::SetVar(int locationID, const Mat3x3f& value) const { glUniformMatrix3fv(locationID, 1, GL_FALSE, &value[0][0]); }
	void Shader::SetVar(int locationID, const Mat2x2f& value) const { glUniformMatrix2fv(locationID, 1, GL_FALSE, &value[0][0]); }

	void Shader::SetVar(int locationID, const Mat4x4d& value) const { glUniformMatrix4dv(locationID, 1, GL_FALSE, &value[0][0]); }
	void Shader::SetVar(int locationID, const Mat3x3d& value) const { glUniformMatrix3dv(locationID, 1, GL_FALSE, &value[0][0]); }
	void Shader::SetVar(int locationID, const Mat2x2d& value) const { glUniformMatrix2dv(locationID, 1, GL_FALSE, &value[0][0]); }

	void Shader::SetTex(int locationID, int unit, unsigned int texID)const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texID);
		glUniform1i(locationID, unit);
	}
}
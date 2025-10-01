#include "Twisted/Rendering/Shader.h"
#include "Logger.h"
#include "Twisted/Rendering/Material.h"
#include "Constants.h"

#include <glad/glad.h>
namespace Twisted
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

	Shader::Shader(ObjectID id) :
		BaseObject(id)
	{

	}

	Shader::Shader(ObjectID id, const ShaderData& shaderData) :
		BaseObject(id)
	{
		Compile(shaderData);
		if (m_isValid)
			DetectUniformVars();
	}

	Shader::~Shader()
	{
		glDeleteShader(m_programID);
	}

	void Shader::Compile(const ShaderData& shaderData)
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

		if (vertexID != 0 && fragmentID != 0 && (geometryID != 0 || shaderData.GeoShader == ""))
		{
			m_programID = glCreateProgram();
			glAttachShader(m_programID, vertexID);
			glAttachShader(m_programID, fragmentID);
			if (geometryID != 0)
				glAttachShader(m_programID, geometryID);
			glLinkProgram(m_programID);

			int success;
			char infoLog[512];

			glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
			if (success != GL_TRUE)
			{
				glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
				TWISTED_ERROR(std::string("Error: Failed to compile shader program!") + infoLog);
			}
			else
			{
				TWISTED_INFO("Shader compile success;");
				m_isValid = true;
			}
		}

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		glDeleteShader(geometryID);
	}

	void Shader::DetectUniformVars()
	{
		glUseProgram(m_programID);

		GLint count = 1;
		GLint varSize;
		GLenum varType;

		const GLsizei nameBufferSize = 16; // maximum name length
		GLchar nameBuffer[nameBufferSize];
		GLsizei nameLength;

		glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &count);
		GLint textureUnit = 0;
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(m_programID, (GLuint)i, nameBufferSize, &nameLength, &varSize, &varType, nameBuffer);
			GLint uniformID = glGetUniformLocation(m_programID, nameBuffer);

			if (varType == GL_SAMPLER_2D)
			{
				glUniform1i(uniformID, textureUnit);
				ShaderTextureVar texVar{ nameBuffer,FromGLShaderType(varType), uniformID, textureUnit++ };
				m_textures.emplace_back(texVar);
			}
			else
			{
				ShaderUniformVar uniVar{ nameBuffer,FromGLShaderType(varType),uniformID };
				m_uniforms.emplace_back(uniVar);
			}
		}
	}

	void Shader::ApplyUniforms(Material* material)const
	{
		for (auto& var : m_uniforms)
		{
			switch (var.Type)
			{
			case ShaderVarType::Bool:
				SetVar(var.UniformID, material->Get<bool>(var.Name));
				break;
			case ShaderVarType::Int:
				SetVar(var.UniformID, material->Get<int>(var.Name));
				break;
			case ShaderVarType::Float:
				SetVar(var.UniformID, material->Get<float>(var.Name));
				break;
			case ShaderVarType::Vec2:
				SetVar(var.UniformID, material->Get<Vec2f>(var.Name));
				break;
			case ShaderVarType::Vec3:
				SetVar(var.UniformID, material->Get<Vec3f>(var.Name));
				break;
			case ShaderVarType::Vec4:
				SetVar(var.UniformID, material->Get<Vec4f>(var.Name));
				break;
			case ShaderVarType::Mat4:
				SetVar(var.UniformID, material->Get<Mat4x4f>(var.Name));
				break;
			default:
				TWISTED_WARN("Unknown shader uniform type");
				break;
			}
		}
		for (auto& var : m_textures)
		{
			//SetTex(var.TextureUnit, material->GetTextureID(var.Name, 0));
		}
	}

	void Shader::Bind()const
	{
		glUseProgram(m_programID);
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
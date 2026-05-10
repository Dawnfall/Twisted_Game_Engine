#ifdef TWISTED_BACKEND_OPENGL

#include "Shader.h"
#include "OpenGL/Shader_OpenGL.h"
#include "OpenGL/ShaderReflection_OpenGL.h"
#include "Data/ShaderData.h"

#include "OpenGL/Texture_OpenGL.h"
#include "Texture.h"

#include "Application/TObject.h"
#include "Utils/GlmUtils.h"
#include "Debug/Logger.h"

#include <glad/glad.h>
#include <string>
#include <vector>
#include <cassert>
#include <variant>

namespace Twisted
{
	Shader::Shader(const std::string& name) :
		TObject(name),
		m_backend(nullptr)
	{
	}

	Shader::~Shader()
	{
	}

	ShaderBackend::ShaderBackend(const ShaderData& shaderData)
	{
		ProgramID = GL::CompileProgram(shaderData);
		if (ProgramID != 0)
		{
			m_reflection = GL::ReflectProgramGL(ProgramID);
			m_bindings = GL::BuildGLProgramBindings(ProgramID, m_reflection);
		}
	}

	ShaderBackend::~ShaderBackend()
	{
		if (ProgramID == 0)
			return;

		glDeleteProgram(ProgramID);
		ProgramID = 0;
		m_reflection = {};
		m_bindings = {};
	}

	const ShaderReflection& Shader::GetReflection() const { return m_backend->m_reflection; }

	void Shader::Clear()
	{
		m_backend = nullptr;
	}

	void Shader::OnDestroy()
	{
		Clear();
	}

	void Shader::SetData(const ShaderData& shaderData)
	{
		m_backend        = std::make_unique<ShaderBackend>(shaderData);
		m_fileReflection = shaderData.reflection;
	}

	void Shader::SetBool(bool val, size_t reflectionIndex)
	{
		glUniform1i(m_backend->m_bindings.values[reflectionIndex].location, val ? 1 : 0);
	}
	void Shader::SetInt(int val, size_t reflectionIndex)
	{
		glUniform1i(m_backend->m_bindings.values[reflectionIndex].location, val);
	}
	void Shader::SetFloat(float val, size_t reflectionIndex)
	{
		glUniform1f(m_backend->m_bindings.values[reflectionIndex].location, val);
	}
	void Shader::SetVec3(const Vec3f& val, size_t reflectionIndex)
	{
		glUniform3f(m_backend->m_bindings.values[reflectionIndex].location, val[0], val[1], val[2]);
	}
	void Shader::SetVec4(const Vec4f& val, size_t reflectionIndex)
	{
		glUniform4f(m_backend->m_bindings.values[reflectionIndex].location, val[0], val[1], val[2], val[3]);
	}
	void Shader::SetMat4(const Mat4x4f& val, size_t reflectionIndex)
	{
		glUniformMatrix4fv(m_backend->m_bindings.values[reflectionIndex].location, 1, GL_FALSE, &val[0][0]);
	}
	void Shader::SetTexture(const Texture* tex, size_t reflectionIndex)
	{
		glActiveTexture(GL_TEXTURE0 + m_backend->m_bindings.textures[reflectionIndex].unit);
		glBindTexture(GL_TEXTURE_2D, tex->GetBackend()->TexID);
		glUniform1i(m_backend->m_bindings.textures[reflectionIndex].location, m_backend->m_bindings.textures[reflectionIndex].unit);
	}
}

namespace Twisted::GL
{
	GLuint CompileProgram(const ShaderData& shaderData)
	{
		if (!shaderData.vert)
		{
			TWISTED_ERROR("Shader compilation failed: missing vertex shader");
			return 0;
		}
		if (!shaderData.frag)
		{
			TWISTED_ERROR("Shader compilation failed: missing fragment shader");
			return 0;
		}

		GLuint vertexID   = GL::CompileShaderCode(GL_VERTEX_SHADER,   "Vertex",   shaderData.vert->c_str());
		GLuint fragmentID = GL::CompileShaderCode(GL_FRAGMENT_SHADER,  "Fragment", shaderData.frag->c_str());
		GLuint geometryID = 0;
		if (shaderData.geom)
			geometryID = GL::CompileShaderCode(GL_GEOMETRY_SHADER, "Geometry", shaderData.geom->c_str());

		GLuint shaderID = GL::LinkProgram(vertexID, fragmentID, geometryID);
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		glDeleteShader(geometryID);

		return shaderID;
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

		auto validateShader = [](GLuint id, const std::string& name, GLenum shaderType)->bool {
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
					TWISTED_ERROR("Error: Failed to compile shader: {} ; {} ; Log: {}", shaderType, name, infoLog);
					return false;
				}
				else
				{
					TWISTED_WARN("Shader compiled with warnings: {} ; {} ; Log: {}", shaderType, name, infoLog);
				}
			}
			return true;
			};

		if (!validateShader(shaderID, shaderName, GL_COMPILE_STATUS))
		{
			glDeleteShader(shaderID);
			shaderID = 0;
		}
		return shaderID;
	}

	GLuint LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID)
	{
		if (vertexID == 0 || fragmentID == 0)
			return 0;

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexID);
		glAttachShader(program, fragmentID);
		if (geometryID != 0)
			glAttachShader(program, geometryID);

		glLinkProgram(program);

		auto validateProgram = [](GLuint id, [[maybe_unused]] const std::string& name)->bool {
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
					TWISTED_ERROR("Error: Failed to compile shader program: {} ; Log: {}", name, infoLog);
					return false;
				}
				else
				{
					TWISTED_WARN("Shader program compiled with warnings: {} ; Log: {}", name, infoLog);
				}
			}
			else
			{
				TWISTED_INFO("Shader program compile success: {}", name);
			}
			return true;
			};

		if (!validateProgram(program, "Shader program"))
		{
			glDeleteProgram(program);
			program = 0;
		}
		return program;
	}
}

#endif


//TODO:... create buffer

//// ---------------------------
//// Create UBO buffer
//// ---------------------------
//glGenBuffers(1, &blockInfo.BufferID);
//glBindBuffer(GL_UNIFORM_BUFFER, blockInfo.BufferID);
//glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW);
//glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//// ---------------------------
//// Assign binding point
//// ---------------------------
//blockInfo.Binding = nextBindingPoint++;
//glUniformBlockBinding(shaderProgram, blockInfo.Index, blockInfo.Binding);
//
//// ---------------------------
//// Bind UBO object to binding point
//// ---------------------------
//glBindBufferBase(GL_UNIFORM_BUFFER, blockInfo.Binding, blockInfo.BufferID);




	//void Shader::ApplyUniforms(const std::vector<ActiveValueUniform>& activeUniforms)
	//{
	//	ShaderBackend* gl = GetBackend();

	//	for (const auto& u : activeUniforms)
	//	{
	//		std::visit([&u,&gl](const auto& value)
	//			{
	//				using T = std::decay_t<decltype(value)>;

	//				if constexpr (std::is_same_v<T, TextureValue>)
	//				{			
	//					glActiveTexture(GL_TEXTURE0 + gl->m_bindings.textures[u.valueIndex].unit);
	//					glBindTexture(GL_TEXTURE_2D, value.tex->GetBackend()->TexID);
	//					glUniform1i(gl->m_bindings.textures[u.valueIndex].location, gl->m_bindings.textures[u.valueIndex].unit);
	//				}
	//				else if constexpr (std::is_same_v<T, bool>);
	//				else if constexpr (std::is_same_v<T, int>) glUniform1i(gl->m_bindings.values[u.valueIndex].location, value);
	//				else if constexpr (std::is_same_v<T, unsigned int>) glUniform1ui(gl->m_bindings.values[u.valueIndex].location, value);
	//				else if constexpr (std::is_same_v<T, float>) glUniform1f(gl->m_bindings.values[u.valueIndex].location, value);
	//				else if constexpr (std::is_same_v<T, double>) glUniform1d(gl->m_bindings.values[u.valueIndex].location, value);

	//				else if constexpr (std::is_same_v<T, Vec2f>) glUniform2f(gl->m_bindings.values[u.valueIndex].location, value[0], value[1]);
	//				else if constexpr (std::is_same_v<T, Vec3f>) glUniform3f(gl->m_bindings.values[u.valueIndex].location, value[0], value[1], value[2]);
	//				else if constexpr (std::is_same_v<T, Vec4f>) glUniform4f(gl->m_bindings.values[u.valueIndex].location, value[0], value[1], value[2], value[3]);

	//				else if constexpr (std::is_same_v<T, Vec2d>) glUniform2d(gl->m_bindings.values[u.valueIndex].location, value[0], value[1]);
	//				else if constexpr (std::is_same_v<T, Vec3d>) glUniform3d(gl->m_bindings.values[u.valueIndex].location, value[0], value[1], value[2]);
	//				else if constexpr (std::is_same_v<T, Vec4d>) glUniform4d(gl->m_bindings.values[u.valueIndex].location, value[0], value[1], value[2], value[3]);

	//				else if constexpr (std::is_same_v<T, Vec2i>) glUniform2i(gl->m_bindings.values[u.valueIndex].location, value[0], value[1]);
	//				else if constexpr (std::is_same_v<T, Vec3i>) glUniform3i(gl->m_bindings.values[u.valueIndex].location, value[0], value[1], value[2]);
	//				else if constexpr (std::is_same_v<T, Vec4i>) glUniform4i(gl->m_bindings.values[u.valueIndex].location, value[0], value[1], value[2], value[3]);

	//				else if constexpr (std::is_same_v<T, Mat4x4f>) glUniformMatrix4fv(gl->m_bindings.values[u.valueIndex].location, 1, GL_FALSE, &value[0][0]);
	//				else if constexpr (std::is_same_v<T, Mat3x3f>) glUniformMatrix3fv(gl->m_bindings.values[u.valueIndex].location, 1, GL_FALSE, &value[0][0]);
	//				else if constexpr (std::is_same_v<T, Mat2x2f>) glUniformMatrix2fv(gl->m_bindings.values[u.valueIndex].location, 1, GL_FALSE, &value[0][0]);

	//				else if constexpr (std::is_same_v<T, Mat4x4d>) glUniformMatrix4dv(gl->m_bindings.values[u.valueIndex].location, 1, GL_FALSE, &value[0][0]);
	//				else if constexpr (std::is_same_v<T, Mat3x3d>) glUniformMatrix3dv(gl->m_bindings.values[u.valueIndex].location, 1, GL_FALSE, &value[0][0]);
	//				else if constexpr (std::is_same_v<T, Mat2x2d>) glUniformMatrix2dv(gl->m_bindings.values[u.valueIndex].location, 1, GL_FALSE, &value[0][0]);

	//				else if constexpr (std::is_same_v<T, Mat4x4i>) { /* no GL integer matrix uniform */ }
	//				else static_assert(!sizeof(T), "Unhandled MaterialValueType alternative");
	//			}, u.val);
	//	}
	//}





#ifndef TWISTED_D3D

#pragma once 
#include "AppCore.h"
#include "Twisted/TObject.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Data/ShaderData.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/OpenGL/ShaderReflection_OpenGL.h"
#include "Twisted/Rendering/Shader.h"

#include <glad/glad.h>
#include <vector>

namespace Twisted
{
	struct TWISTED_API ShaderBackend
	{
		GLuint ProgramID = 0;
		ShaderReflection m_reflection; //indices match
		GL::GLProgramBindings m_bindings; //
	};

	inline const ShaderReflection& Shader::GetReflection() const { return m_backend->m_reflection; }
}


namespace Twisted::GL
{
	[[nodiscard]] GLuint TWISTED_API CompileProgram(const ShaderData& shaderData);
	[[nodiscard]] GLuint TWISTED_API CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode);
	[[nodiscard]] GLuint TWISTED_API LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID);

	inline void TWISTED_API BindShader(GLuint shader) { glUseProgram(shader); }
	inline void TWISTED_API UnbindShader() { glUseProgram(0); }

	template<typename T>
	void SetUniformBuffer(const T& data, GLuint id)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

#endif

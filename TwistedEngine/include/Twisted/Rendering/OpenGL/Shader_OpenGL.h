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
		GL::GLProgramBindings m_bindings;

		ShaderBackend(const ShaderData& shaderData);
		~ShaderBackend();

		bool UsesBlock(GLuint bindingPoint) const
		{
			for (const auto& b : m_bindings.blocks)
				if (b.bindingPoint == bindingPoint) return true;
			return false;
		}
	};

}


namespace Twisted::GL
{
	[[nodiscard]] GLuint TWISTED_API CompileProgram(const ShaderData& shaderData);
	[[nodiscard]] GLuint TWISTED_API CompileShaderCode(GLenum shaderType, const char* shaderName, const char* shaderCode);
	[[nodiscard]] GLuint TWISTED_API LinkProgram(GLuint vertexID, GLuint fragmentID, GLuint geometryID);

	inline void TWISTED_API BindShader(GLuint shader) { glUseProgram(shader); }
	inline void TWISTED_API UnbindShader() { glUseProgram(0); }

}

#endif

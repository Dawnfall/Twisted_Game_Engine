#ifdef TWISTED_BACKEND_OPENGL

#pragma once 
#include "AppCore.h"
#include "Application/TObject.h"
#include "Utils/GlmUtils.h"
#include "Data/ShaderData.h"
#include "Material.h"
#include "OpenGL/ShaderReflection_OpenGL.h"
#include "Shader.h"

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

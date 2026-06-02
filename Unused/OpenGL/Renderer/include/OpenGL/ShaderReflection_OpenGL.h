#ifdef TWISTED_BACKEND_OPENGL

#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <cassert>

#include "Data/ShaderData.h"
#include "Debug/Logger.h"

namespace Twisted::GL
{
	struct GLValueBinding
	{
		size_t valueDescIndex = 0;   // index into reflection.values
		GLint location = -1;		 // GL-only
	};

	struct GLTextureBinding
	{
		size_t textureDescIndex = 0; // index into reflection.textures
		GLint location = -1;		 // GL-only
		GLint unit = -1;			 // assigned by you
	};

	struct GLBlockBinding
	{
		size_t blockDescIndex = 0;   // index into reflection.blocks
		GLuint blockIndex = 0;		 // glGetUniformBlockIndex result (program-specific)
		GLuint bindingPoint = 0;	 // assigned by you (or from layout(binding=))
	};

	struct GLProgramBindings
	{
		std::vector<GLValueBinding> values;
		std::vector<GLTextureBinding> textures;
		std::vector<GLBlockBinding> blocks;
	};
}

namespace Twisted::GL
{
	std::string GetResName(GLuint program, GLenum iface, GLuint idx);

	// Strip trailing "[0]" so materials can key by base name.
	std::string CanonicalUniformName(std::string name);

	bool IsSamplerType(GLenum t);

	EShaderValueType FromGLValueType(GLenum t);

    std::string UniformQueryName(const std::string& baseName, uint32_t arrayCount);

	void ReflectBlockMembersGL(GLuint program, BlockDesc& block, GLuint blockResourceIndex);
	ShaderReflection ReflectProgramGL(GLuint program);

	GLProgramBindings BuildGLProgramBindings(GLuint program, const ShaderReflection& refl, GLint startingTextureUnit = 0); // allow reserving units for engine globals)
}
#endif

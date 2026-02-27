#ifndef TWISTED_D3D

#include "Twisted/Rendering/OpenGL/ShaderReflection_OpenGL.h"

namespace Twisted::GL
{
	std::string GetResName(GLuint program, GLenum iface, GLuint idx)
	{
		GLint len = 0;
		const GLenum p = GL_NAME_LENGTH;
		glGetProgramResourceiv(program, iface, idx, 1, &p, 1, nullptr, &len);
		if (len <= 0)
			return {};

		std::string s(len, '\0');
		glGetProgramResourceName(program, iface, idx, len, nullptr, s.data());
		if (!s.empty() && s.back() == '\0')
			s.pop_back();
		return s;
	}

	std::string CanonicalUniformName(std::string name)
	{
		// Common GL reflection format for arrays: "uLights[0]"
		if (name.size() >= 3 && name.compare(name.size() - 3, 3, "[0]") == 0)
			name.resize(name.size() - 3);
		return name;
	}

	bool IsSamplerType(GLenum t)
	{
		switch (t)
		{
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
		case GL_SAMPLER_1D_ARRAY:
		case GL_SAMPLER_2D_ARRAY:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_CUBE_SHADOW:
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
			return true;
		default:
			return false;
		}
	}

	EShaderValueType FromGLValueType(GLenum t)
	{
		switch (t)
		{
			// Scalars
		case GL_BOOL:   return EShaderValueType::BOOL;
		case GL_INT:    return EShaderValueType::INT;
		case GL_UNSIGNED_INT: return EShaderValueType::UNSIGNED_INT;
		case GL_FLOAT:  return EShaderValueType::FLOAT;
		case GL_DOUBLE: return EShaderValueType::DOUBLE;

			// Float vectors
		case GL_FLOAT_VEC2: return EShaderValueType::VEC2_F;
		case GL_FLOAT_VEC3: return EShaderValueType::VEC3_F;
		case GL_FLOAT_VEC4: return EShaderValueType::VEC4_F;

			// Double vectors
		case GL_DOUBLE_VEC2: return EShaderValueType::VEC2_D;
		case GL_DOUBLE_VEC3: return EShaderValueType::VEC3_D;
		case GL_DOUBLE_VEC4: return EShaderValueType::VEC4_D;

			// Int vectors
		case GL_INT_VEC2: return EShaderValueType::VEC2_I;
		case GL_INT_VEC3: return EShaderValueType::VEC3_I;
		case GL_INT_VEC4: return EShaderValueType::VEC4_I;

			// UInt vectors
		case GL_UNSIGNED_INT_VEC2: return EShaderValueType::VEC2_U;
		case GL_UNSIGNED_INT_VEC3: return EShaderValueType::VEC3_U;
		case GL_UNSIGNED_INT_VEC4: return EShaderValueType::VEC4_U;

			// Float matrices
		case GL_FLOAT_MAT2: return EShaderValueType::MAT2x2_F;
		case GL_FLOAT_MAT3: return EShaderValueType::MAT3x3_F;
		case GL_FLOAT_MAT4: return EShaderValueType::MAT4x4_F;

			// Double matrices
		case GL_DOUBLE_MAT2: return EShaderValueType::MAT2x2_D;
		case GL_DOUBLE_MAT3: return EShaderValueType::MAT3x3_D;
		case GL_DOUBLE_MAT4: return EShaderValueType::MAT4x4_D;

		case GL_SAMPLER_2D:
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_2D_SHADOW:
			return EShaderValueType::TEX_2D;

			// Add more later:
			// case GL_SAMPLER_CUBE: return EShaderValueType::TEX_CUBE;
			// case GL_SAMPLER_2D_ARRAY: return EShaderValueType::TEX_2D_ARRAY;
			// case GL_SAMPLER_3D: return EShaderValueType::TEX_3D;


		default: return EShaderValueType::UNKNOWN;
		}
	}

	std::string UniformQueryName(const std::string& baseName, uint32_t arrayCount)
	{
		// OpenGL typically expects querying arrays via "name[0]"
		if (arrayCount > 1) return baseName + "[0]";
		return baseName;
	}

	void ReflectBlockMembersGL(GLuint program, BlockDesc& block, GLuint blockResourceIndex)
	{
		// How many active members are in this block?
		GLint numActive = 0;
		{
			GLenum prop = GL_NUM_ACTIVE_VARIABLES;
			glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, blockResourceIndex,
				1, &prop, 1, nullptr, &numActive);
		}
		if (numActive <= 0)
			return;

		// Get the GL_UNIFORM resource indices for the members
		std::vector<GLint> memberUniformIndices((size_t)numActive);
		{
			GLenum prop = GL_ACTIVE_VARIABLES;
			glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, blockResourceIndex,
				1, &prop, numActive, nullptr, memberUniformIndices.data());
		}

		std::unordered_set<std::string> seen;

		for (GLint uIndex : memberUniformIndices)
		{
			GLenum props[] = { GL_TYPE, GL_ARRAY_SIZE, GL_OFFSET };
			GLint  vals[3] = {};
			glGetProgramResourceiv(program, GL_UNIFORM, (GLuint)uIndex, 3, props, 3, nullptr, vals);

			GLenum glType = (GLenum)vals[0];
			GLint  arraySize = vals[1];
			GLint  offset = vals[2];
			if (arraySize <= 0)
				arraySize = 1;

			std::string rawName = GetResName(program, GL_UNIFORM, (GLuint)uIndex);
			if (rawName.empty())
				continue;

			// Make member name UI-friendly. Ideally this returns just "member" not "Block.member".
			std::string memberName = CanonicalUniformName(std::move(rawName));

			// Avoid duplicates like "arr" vs "arr[0]"
			if (!seen.insert(memberName).second)
				continue;

			BlockValueDesc m;
			m.name = std::move(memberName);
			m.type = FromGLValueType(glType);
			m.arrayCount = (uint32_t)arraySize;
			m.offsetBytes = (uint32_t)offset;

			block.members.emplace_back(std::move(m));
		}

		// Optional: stable order for UI/debug
		//std::sort(block.members.begin(), block.members.end(),
		//	[](const BlockValueDesc& a, const BlockValueDesc& b) {
		//		return a.offsetBytes < b.offsetBytes;
		//	});
	}

	ShaderReflection ReflectProgramGL(GLuint program)
	{
		ShaderReflection reflection;

		GLint uniformCount = 0;
		glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformCount);

		//int nextTextureUnit = 0;
		for (GLuint i = 0; i < (GLuint)uniformCount; ++i)
		{
			GLenum propsA[] = { GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX, GL_ARRAY_SIZE };
			GLint  valsA[4] = {};
			glGetProgramResourceiv(program, GL_UNIFORM, i, 4, propsA, 4, nullptr, valsA);

			GLenum glType = (GLenum)valsA[0];
			GLint  location = valsA[1];
			GLint  blockIdx = valsA[2];
			GLint  arrSize = valsA[3];
			if (arrSize <= 0)
				arrSize = 1;

			std::string rawName = GetResName(program, GL_UNIFORM, i);
			if (rawName.empty() || blockIdx != -1 || location < 0)
				continue;

			ValueDesc valueDesc;
			valueDesc.name = CanonicalUniformName(std::move(rawName));
			valueDesc.arrayCount = (uint32_t)arrSize;
			valueDesc.type = FromGLValueType(glType);

			if (IsSamplerType(glType)) //texture 
				reflection.texIndices.push_back(reflection.values.size());
			reflection.values.emplace_back(std::move(valueDesc));
		}

		// Phase 2: uniform blocks + members
		GLint blockCount = 0;
		glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &blockCount);

		for (GLuint b = 0; b < (GLuint)blockCount; ++b)
		{
			BlockDesc block;
			block.name = GetResName(program, GL_UNIFORM_BLOCK, b);

			GLenum props[] = { GL_BUFFER_DATA_SIZE, GL_ARRAY_SIZE };
			GLint  vals[2] = { 0, 1 };
			glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, b, 2, props, 2, nullptr, vals);

			block.sizeBytes = (uint32_t)vals[0];
			// if you add it later: block.arrayCount = (uint32_t)std::max(vals[1], 1);

			ReflectBlockMembersGL(program, block, b); // <-- call goes here

			reflection.blocks.emplace_back(std::move(block));
		}
		return reflection;
	}

	// If you use layout(binding=N) in GLSL and want to keep those bindings,
	// you can add code to query GL_BUFFER_BINDING and use it instead of assigning.
	GLProgramBindings BuildGLProgramBindings(
		GLuint program,
		const ShaderReflection& refl,
		GLint startingTextureUnit // allow reserving units for engine globals
	)
	{
		GLProgramBindings out;

		// -------- Values (loose uniforms) --------
		out.values.reserve(refl.values.size());
		for (size_t i = 0; i < (size_t)refl.values.size(); ++i)
		{
			const auto& v = refl.values[i];
			const std::string qname = UniformQueryName(v.name, v.arrayCount);

			GLint loc = glGetUniformLocation(program, qname.c_str());
			if (loc < 0)
				continue; // optimized out / not active

			GLValueBinding b;
			b.valueDescIndex = i;
			b.location = loc;
			out.values.emplace_back(b);
		}

		// -------- Textures (samplers) --------
		out.textures.reserve(refl.texIndices.size());
		GLint nextUnit = startingTextureUnit;

		for (size_t i = 0; i < (size_t)refl.texIndices.size(); ++i)
		{
			const auto& t = refl.values[refl.texIndices[i]];
			const std::string qname = UniformQueryName(t.name, t.arrayCount);

			GLint loc = glGetUniformLocation(program, qname.c_str());
			if (loc < 0)
				continue;

			GLTextureBinding b;
			b.textureDescIndex = i;
			b.location = loc;
			b.unit = nextUnit;

			// Tell the shader which texture units to sample from.
			// Use direct state access (glProgramUniform*), no need to glUseProgram.
			if (t.arrayCount == 1)
			{
				glProgramUniform1i(program, loc, b.unit);
			}
			else
			{
				std::vector<GLint> units(t.arrayCount);
				for (uint32_t k = 0; k < t.arrayCount; ++k)
					units[k] = b.unit + (GLint)k;

				glProgramUniform1iv(program, loc, (GLsizei)t.arrayCount, units.data());
			}

			nextUnit += (GLint)t.arrayCount;
			out.textures.emplace_back(b);
		}

		// -------- Uniform Blocks (UBOs) --------
		out.blocks.reserve(refl.blocks.size());

		for (size_t i = 0; i < (size_t)refl.blocks.size(); ++i)
		{
			const auto& blk = refl.blocks[i];

			GLuint uboBlockIndex = glGetUniformBlockIndex(program, blk.name.c_str());
			if (uboBlockIndex == GL_INVALID_INDEX)
				continue; // not present/active in this program

			GLint binding = 0;
			{
				GLenum prop = GL_BUFFER_BINDING;
				glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, uboBlockIndex,
					1, &prop, 1, nullptr, &binding);
			}

			GLBlockBinding b;
			b.blockDescIndex = i;
			b.blockIndex = uboBlockIndex;
			b.bindingPoint = (GLuint)binding;

			// Optional safety: enforce/ensure mapping (should already be set)
			glUniformBlockBinding(program, b.blockIndex, b.bindingPoint);

			out.blocks.emplace_back(b);
		}
		return out;
	}
}

#endif
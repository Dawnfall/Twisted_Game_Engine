#pragma once
#include "AppCore.h"
#include "Twisted/TObject.h"
#include "Utils/GlmUtils.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace Twisted
{
	enum class TWISTED_API ShaderVarType : uint8_t
	{
		BOOL, UNSIGNED_INT, INT, FLOAT, DOUBLE,
		VEC4_F, VEC3_F, VEC2_F,
		VEC4_D, VEC3_D, VEC2_D,
		VEC4_I, VEC3_I, VEC2_I,
		VEC4_U, VEC3_U, VEC2_U,
		MAT4x4_F, MAT3x3_F, MAT2x2_F,
		MAT4x4_D, MAT3x3_D, MAT2x2_D,
		SAMPLER2D,
		UNKNOWN
	};

	struct ShaderUniformVar
	{
		std::string Name = "";
		ShaderVarType Type = ShaderVarType::UNKNOWN;
		int TextureUnit = 0;
		int UniformID = 0;
	};

	struct ShaderUniformBlock
	{
		std::string Name = "";
		unsigned int Index = 0;       // block index inside the shader
		unsigned int Binding = 0;     // binding point we assign
		unsigned int BufferID = 0;    // the generated UBO
		int Size = 0;       // block size in bytes
	};

	struct ShaderData
	{
		std::string VertShader;
		std::string FragShader;
		std::string GeoShader;
	};

	template<typename T>
	struct TWISTED_API ShaderValue
	{
		int ID = -1;
		T value = T();
	};

	struct TWISTED_API Shader :public TObject
	{
		Shader(const std::string& name);
		void OnDestroy()override;
		bool IsValid()const { return ProgramID != 0; }

		unsigned int ProgramID = 0;
		std::vector<ShaderUniformVar> Uniforms;
		std::vector<ShaderUniformBlock> UniformBlocks;
	};
}




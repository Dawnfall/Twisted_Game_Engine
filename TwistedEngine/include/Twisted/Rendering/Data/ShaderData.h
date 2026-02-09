#pragma once
#include "AppCore.h"
#include <string>

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
}
#pragma once
#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Rendering/Texture.h"
#include "Debug/Logger.h"
#include "Utils/WPtr.h"

#include <string>
#include <unordered_map>
#include <variant>

namespace Twisted
{
	enum class TWISTED_API EShaderValueType : uint8_t
	{
		BOOL, UNSIGNED_INT, INT, FLOAT, DOUBLE,
		VEC4_F, VEC3_F, VEC2_F,
		VEC4_D, VEC3_D, VEC2_D,
		VEC4_I, VEC3_I, VEC2_I,
		VEC4_U, VEC3_U, VEC2_U,
		MAT4x4_F, MAT3x3_F, MAT2x2_F,
		MAT4x4_D, MAT3x3_D, MAT2x2_D,
		TEX_2D,
		UNKNOWN
	};

	struct TextureValue
	{
		WPtr<Texture> tex;
	};

	using ShaderValueType = std::variant<
		int, float, double, bool,
		Vec2i, Vec2f, Vec2d,
		Vec3i, Vec3f, Vec3d,
		Vec4i, Vec4f, Vec4d,
		Mat4x4i, Mat4x4f, Mat4x4d,
		TextureValue
	>;

	struct ValueDesc 
	{
		std::string name;
		EShaderValueType type = EShaderValueType::UNKNOWN;
		uint32_t arrayCount = 1;
	};

	struct BlockValueDesc
	{
		std::string name;
		EShaderValueType type = EShaderValueType::UNKNOWN;
		uint32_t arrayCount = 1;

		uint32_t offsetBytes = 0;
	};

	struct BlockDesc 
	{
		std::string name;
		uint32_t sizeBytes = 0;
		std::vector<BlockValueDesc> members;
	};

	struct ShaderReflection
	{
		std::vector<ValueDesc> values;
		std::vector<size_t> texIndices;
		std::vector<BlockDesc> blocks;
	};

	struct ShaderData
	{
		std::string VertShader;
		std::string FragShader;
		std::string GeoShader;
	};

	inline ShaderValueType GetDefaultValue(EShaderValueType shaderValType)
	{
		switch (shaderValType)
		{
		case EShaderValueType::BOOL:
			return false;
		case EShaderValueType::FLOAT:
			return 0.0f;
		case EShaderValueType::INT:
			return 0;
		case EShaderValueType::MAT2x2_F:
			return Mat2x2f{};
		case EShaderValueType::MAT3x3_F:
			return Mat3x3f{};
		case EShaderValueType::MAT4x4_F:
			return Mat4x4f{};
		case EShaderValueType::VEC2_F:
			return Vec2f{};
		case EShaderValueType::VEC3_F:
			return Vec3f{};
		case EShaderValueType::VEC4_F:
			return Vec4f{};
		default:
			TWISTED_ERROR("Unsupported shader uniform value type");
			throw std::exception("Unsupported type");
		}
	}

	inline void resetShaderValue(ShaderValueType& val)
	{
		std::visit([](auto&& v) {
			using T = std::decay_t<decltype(v)>;
			v = T{};
			}, val);
	}
}
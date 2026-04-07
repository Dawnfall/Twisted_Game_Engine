#pragma once
#include "Utils/GlmUtils.h"

#include <cstdint>
#include <cassert>

namespace Twisted
{
	// Base for all UBO structs — carries the binding point as a compile-time constant.
	// Empty base, so no storage overhead (EBO applies).
	template<uint32_t Binding>
	struct ShaderBuffer
	{
		static constexpr uint32_t kBindingPoint = Binding;
	};

	// -------------------------------------------------------------------------
	// MVP  (binding = 0)
	// -------------------------------------------------------------------------
	// std140: mat4 is 4 * vec4 columns = 64 bytes, 16-byte aligned. No padding needed.
	struct ShaderMVPBuffer : ShaderBuffer<0>
	{
		Mat4x4f ModelMatrix;      // offset   0
		Mat4x4f ViewMatrix;       // offset  64
		Mat4x4f ProjectionMatrix; // offset 128
	};                            // total  192 bytes
	static_assert(sizeof(ShaderMVPBuffer) == 192);

	// -------------------------------------------------------------------------
	// Light element types  (std140-correct)
	// All use Vec4f instead of Vec3f to avoid std140 vec3 alignment headaches.
	// -------------------------------------------------------------------------
	struct GPUDirLight
	{
		Vec4f direction;  // xyz = direction, w unused  | offset  0
		Vec4f lightColor; // xyz = color,     w unused  | offset 16
		float intensity;                               // offset 32
		float _pad[3];                                 // offset 36  -> 48
	};
	static_assert(sizeof(GPUDirLight) == 48);

	struct GPUPointLight
	{
		Vec4f position;   // xyz = position,  w unused  | offset  0
		Vec4f lightColor; // xyz = color,     w unused  | offset 16
		float intensity;                               // offset 32
		float radius;                                  // offset 36
		float _pad[2];                                 // offset 40  -> 48
	};
	static_assert(sizeof(GPUPointLight) == 48);

	struct GPUSpotLight
	{
		Vec4f position;   // xyz = position,  w unused  | offset  0
		Vec4f direction;  // xyz = direction, w unused  | offset 16
		Vec4f lightColor; // xyz = color,     w unused  | offset 32
		float intensity;                               // offset 48
		float radius;                                  // offset 52
		float innerCos;                                // offset 56
		float outerCos;                                // offset 60
	};                                                 // total  64 bytes
	static_assert(sizeof(GPUSpotLight) == 64);

	// -------------------------------------------------------------------------
	// Lights buffer  (binding = 1)
	// -------------------------------------------------------------------------
	inline constexpr uint32_t kMaxDirLights   = 8;
	inline constexpr uint32_t kMaxPointLights = 64;
	inline constexpr uint32_t kMaxSpotLights  = 32;

	struct ShaderLightsBuffer : ShaderBuffer<1>
	{
		GPUDirLight   dirLights[kMaxDirLights];    // offset    0  (8  * 48  = 384)
		int           dirLightCount;               // offset  384
		float         _pad0[3];                    // offset  388  -> 400

		GPUPointLight pointLights[kMaxPointLights]; // offset  400  (64 * 48  = 3072)
		int           pointLightCount;              // offset 3472
		float         _pad1[3];                     // offset 3476  -> 3488

		GPUSpotLight  spotLights[kMaxSpotLights];  // offset 3488  (32 * 64  = 2048)
		int           spotLightCount;              // offset 5536
		float         _pad2[3];                    // offset 5540  -> 5552
	};
	static_assert(sizeof(ShaderLightsBuffer) == 5552);
}

/*
GLSL reference:

layout(std140, binding = 0) uniform ShaderMVPBuffer
{
	mat4 uModel;
	mat4 uView;
	mat4 uProj;
};

struct GPUDirLight   { vec4 direction; vec4 lightColor; float intensity; float _pad[3]; };
struct GPUPointLight { vec4 position;  vec4 lightColor; float intensity; float radius; float _pad[2]; };
struct GPUSpotLight  { vec4 position;  vec4 direction;  vec4 lightColor; float intensity; float radius; float innerCos; float outerCos; };

layout(std140, binding = 1) uniform ShaderLightsBuffer
{
	GPUDirLight   dirLights[8];
	int           dirLightCount;

	GPUPointLight pointLights[64];
	int           pointLightCount;

	GPUSpotLight  spotLights[32];
	int           spotLightCount;
};
*/

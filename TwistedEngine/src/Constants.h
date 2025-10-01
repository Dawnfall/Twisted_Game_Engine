#pragma once

#include "Utils/GlmUtils.h"
#include <filesystem>

namespace Constants
{
	inline constexpr  Vec3f Forward(0.0f, 0.0f, -1.0f);
	inline constexpr  Vec3f Back(0.0f, 0.0f, 1.0f);
	inline constexpr  Vec3f Right(1.0f, 0.0f, 0.0f);
	inline constexpr  Vec3f Left(-1.0f, 0.0f, 0.0f);
	inline constexpr  Vec3f Up(0.0f, 1.0f, 0.0f);
	inline constexpr  Vec3f Down(0.0f, -1.0f, 0.0f);
	 
	inline constexpr  Vec4f Vec4Zero(0.0f);
	inline constexpr  Vec3f Vec3Zero(0.0f);
	inline constexpr  Vec2f Vec2Zero(0.0f);
	 
	inline constexpr  Vec4f Vec4One(1.0f);
	inline constexpr  Vec3f Vec3One(1.0f);
	inline constexpr  Vec2f Vec2One(1.0f);
	 
	inline constexpr  Mat4x4f IdentityMat(1.0f);
	inline constexpr  Mat4x4f ZeroMat(0.0f);
	 
	inline constexpr  Quat IdentityQuat(1.0f, 0.0f, 0.0f, 0.0f);
}

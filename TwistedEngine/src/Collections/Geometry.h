#pragma once

#include "twistedpch.h"
#include "AppCore.h"

namespace Twisted
{
	using Quat = glm::quat;

	using Vec2d = glm::vec<2, double>;
	using Vec3d = glm::vec<3, double>;
	using Vec4d = glm::vec<4, double>;

	using Vec2f = glm::vec<2, float>;
	using Vec3f = glm::vec<3, float>;
	using Vec4f = glm::vec<4, float>;

	using Vec2i = glm::vec<2, int>;
	using Vec3i = glm::vec<3, int>;
	using Vec4i = glm::vec<4, int>;

	using Mat4x4f = glm::mat<4, 4, float>;

	struct TWISTED_API Vertex {
		Vec3f Position;
		Vec3f Normal;
		Vec2f TexCoord;
	};

	//**************
	// Directions

	namespace Directions
	{
		const Vec3f Forward(0.0f, 0.0f, -1.0f);
		const Vec3f Back(0.0f, 0.0f, 1.0f);
		const Vec3f Right(1.0f, 0.0f, 0.0f);
		const Vec3f Left(-1.0f, 0.0f, 0.0f);
		const Vec3f Up(0.0f, 1.0f, 0.0f);
		const Vec3f Down(0.0f, -1.0f, 0.0f);

		const Vec3f Zero(0.0f);
		const Vec3f One(1.0f);

		const Mat4x4f IdentityMat(1.0f);
		const Mat4x4f ZeroMat(0.0f);

		const Quat IdentityQuat(1.0f, 0.0f, 0.0f, 0.0f);
	}
}
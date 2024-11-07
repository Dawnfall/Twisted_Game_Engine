#pragma once

#include "twistedpch.h"

namespace Twisted
{
	typedef glm::quat Quat;

	typedef glm::vec<2, double> Vec2d;
	typedef glm::vec<3, double> Vec3d;
	typedef glm::vec<4, double> Vec4d;

	typedef glm::vec<2, float> Vec2f;
	typedef glm::vec<3, float> Vec3f;
	typedef glm::vec<4, float> Vec4f;

	typedef glm::vec<2, int> Vec2i;
	typedef glm::vec<3, int> Vec3i;
	typedef glm::vec<4, int> Vec4i;

	typedef glm::mat<4, 4, float> Mat4x4f;



	struct Vertex {
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
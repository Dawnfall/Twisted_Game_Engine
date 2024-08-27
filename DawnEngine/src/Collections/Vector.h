#pragma once

#include "glm.hpp"
#include <gtc/quaternion.hpp>

namespace Dawn
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

	typedef glm::mat<4,4,float> Mat4x4f;
}
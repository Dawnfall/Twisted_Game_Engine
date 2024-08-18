#include "Directions.h"

namespace Dawn::Collections
{
	const Vec3f Directions::Forward(0.0f, 0.0f, -1.0f);
	const Vec3f Directions::Back(0.0f, 0.0f, 1.0f);
	const Vec3f Directions::Right(1.0f, 0.0f, 0.0f);
	const Vec3f Directions::Left(-1.0f, 0.0f, 0.0f);
	const Vec3f Directions::Up(0.0f, 1.0f, 0.0f);
	const Vec3f Directions::Down(0.0f, -1.0f, 0.0f);

	const Vec3f Directions::Zero(0.0f);
	const Vec3f Directions::One(1.0f);

	const Mat4x4f Directions::ZeroMat(0.0f);
	const Mat4x4f Directions::IdentityMat(1.0f);

	const Quat Directions::IdentityQuat(1.0f, 0.0f, 0.0f, 0.0f);
}
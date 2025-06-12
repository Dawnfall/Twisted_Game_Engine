#pragma once
#include "AppCore.h"

//glm
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>

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

namespace Utils
{
	std::string TWISTED_API toString(const glm::vec2& vec);
    std::string TWISTED_API toString(glm::ivec2& vec);
    std::string TWISTED_API toString(const glm::vec3& vec);
	std::string TWISTED_API toString(const glm::vec4& vec);
	std::string TWISTED_API toString(const glm::mat3& mat);

	glm::vec2 TWISTED_API fromStringVec2f(const std::string& str);
	glm::ivec2 TWISTED_API fromStringVec2i(const std::string& str);
    glm::vec3 TWISTED_API fromStringVec3(const std::string& str);
    glm::vec4 TWISTED_API fromStringVec4(const std::string& str);
    glm::mat3 TWISTED_API fromStringMat3(const std::string& str);
    glm::mat4 TWISTED_API fromStringMat4(const std::string& str);
}
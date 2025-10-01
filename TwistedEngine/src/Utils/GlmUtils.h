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

#include <yaml-cpp/yaml.h>

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

template<>
struct YAML::convert<glm::vec<2, float>>
{
	static YAML::Node encode(const glm::vec<2, float>& rhs) {
		YAML::Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		return node;
	}

	static bool decode(const YAML::Node& node, glm::vec<2, float>& rhs) {
		if (!node.IsSequence() || node.size() != 2)
			return false;
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		return true;
	}
};

template<>
struct YAML::convert<glm::vec<3, float>>
{
	static YAML::Node encode(const glm::vec<3, float>& rhs) {
		YAML::Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		return node;
	}

	static bool decode(const YAML::Node& node, glm::vec<3, float>& rhs) {
		if (!node.IsSequence() || node.size() != 3)
			return false;
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		return true;
	}
};

template<>
struct YAML::convert<glm::vec<4, float>>
{
	static YAML::Node encode(const glm::vec<4, float>& rhs) {
		YAML::Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		return node;
	}

	static bool decode(const YAML::Node& node, glm::vec<4, float>& rhs) {
		if (!node.IsSequence() || node.size() != 4)
			return false;
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}
};

template<>
struct YAML::convert<glm::mat3>
{
	static YAML::Node encode(const glm::mat3& rhs) {
		YAML::Node node;
		for (int col = 0; col < 3; ++col) {
			YAML::Node colNode;
			for (int row = 0; row < 3; ++row) {
				colNode.push_back(rhs[col][row]);
			}
			node.push_back(colNode);
		}
		return node;
	}

	static bool decode(const YAML::Node& node, glm::mat3& rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;
		for (int col = 0; col < 3; ++col) {
			const YAML::Node& colNode = node[col];
			if (!colNode.IsSequence() || colNode.size() != 3)
				return false;
			for (int row = 0; row < 3; ++row) {
				rhs[col][row] = colNode[row].as<float>();
			}
		}
		return true;
	}
};

template<>
struct YAML::convert<glm::mat4>
{
	static YAML::Node encode(const glm::mat4& rhs)
	{
		YAML::Node node;
		for (int col = 0; col < 4; ++col)
		{
			YAML::Node colNode;
			for (int row = 0; row < 4; ++row)
			{
				colNode.push_back(rhs[col][row]);
			}
			node.push_back(colNode);
		}
		return node;
	}

	static bool decode(const YAML::Node& node, glm::mat4& rhs)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;
		for (int col = 0; col < 4; ++col)
		{
			const YAML::Node& colNode = node[col];
			if (!colNode.IsSequence() || colNode.size() != 4)
				return false;
			for (int row = 0; row < 4; ++row)
			{
				rhs[col][row] = colNode[row].as<float>();
			}
		}
		return true;
	}
};
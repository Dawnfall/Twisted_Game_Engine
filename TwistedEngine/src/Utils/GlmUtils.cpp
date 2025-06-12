#include "GlmUtils.h"
#include <string>
#include <glm/glm.hpp>
#include <sstream>

namespace Utils
{
    std::string toString(const glm::vec2& vec) {
        std::ostringstream oss;
        oss << "vec2(" << vec.x << ", " << vec.y << ")";
        return oss.str();
    }

    std::string toString(glm::ivec2& vec)
    {
        std::ostringstream oss;
        oss << "vec2(" << vec.x << ", " << vec.y << ")";
        return oss.str();
    }

    // Convert glm::vec3 to string
    std::string toString(const glm::vec3& vec) {
        std::ostringstream oss;
        oss << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return oss.str();
    }

    // Convert glm::vec4 to string
    std::string toString(const glm::vec4& vec) {
        std::ostringstream oss;
        oss << "vec4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return oss.str();
    }

    // Convert glm::mat3 to string
    std::string toString(const glm::mat3& mat) {
        std::ostringstream oss;
        oss << "mat3(\n";
        for (int i = 0; i < 3; ++i) {
            oss << "  " << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << "\n";
        }
        oss << ")";
        return oss.str();
    }


    // Convert string to glm::vec2
    glm::vec2 fromStringVec2f(const std::string& str) {
        glm::vec2 vec;
        if (sscanf(str.c_str(), "vec2(%f, %f)", &vec.x, &vec.y) != 2) {
            throw std::invalid_argument("Invalid format for vec2");
        }
        return vec;
    }
    glm::ivec2 fromStringVec2i(const std::string& str)
    {
        glm::ivec2 vec;
        if (sscanf(str.c_str(), "vec2(%f, %f)", &vec.x, &vec.y) != 2) {
            throw std::invalid_argument("Invalid format for vec2");
        }
        return vec;
    }

    // Convert string to glm::vec3
    glm::vec3 fromStringVec3(const std::string& str) {
        glm::vec3 vec;
        if (sscanf(str.c_str(), "vec3(%f, %f, %f)", &vec.x, &vec.y, &vec.z) != 3) {
            throw std::invalid_argument("Invalid format for vec3");
        }
        return vec;
    }

    // Convert string to glm::vec4
    glm::vec4 fromStringVec4(const std::string& str) {
        glm::vec4 vec;
        if (sscanf(str.c_str(), "vec4(%f, %f, %f, %f)", &vec.x, &vec.y, &vec.z, &vec.w) != 4) {
            throw std::invalid_argument("Invalid format for vec4");
        }
        return vec;
    }

    // Convert string to glm::mat3
    glm::mat3 fromStringMat3(const std::string& str) {
        glm::mat3 mat;
        if (sscanf(str.c_str(), "mat3(\n  %f, %f, %f\n  %f, %f, %f\n  %f, %f, %f\n)",
            &mat[0][0], &mat[0][1], &mat[0][2],
            &mat[1][0], &mat[1][1], &mat[1][2],
            &mat[2][0], &mat[2][1], &mat[2][2]) != 9) {
            throw std::invalid_argument("Invalid format for mat3");
        }
        return mat;
    }

    // Convert string to glm::mat4
    glm::mat4 fromStringMat4(const std::string& str) {
        glm::mat4 mat;
        if (sscanf(str.c_str(), "mat4(\n  %f, %f, %f, %f\n  %f, %f, %f, %f\n  %f, %f, %f, %f\n  %f, %f, %f, %f\n)",
            &mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3],
            &mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
            &mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3],
            &mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]) != 16) {
            throw std::invalid_argument("Invalid format for mat4");
        }
        return mat;
    }

}
#pragma once
#include <string>
#include <cstdint>
#include <utility>

namespace Twisted
{
	inline const std::string DebugNormalShaderName = "debug_normals_shader";
	inline constexpr std::pair<uint64_t, uint64_t> DebugNormalShaderUUID{ 0x7AB3E1D4C298F0EEULL, 0x44D1A99C08F3C2B1ULL };

	inline const std::string DebugNormalShader =
R"(###VERT###
#version 430 core
#import "MVP_Core_Vert"

void main()
{
    vNormal = GetNormalMatrix() * aNormal;
    gl_Position = TransformMVP(aPos);
}
###VERT/###

###FRAG###
#version 430 core
in vec3 vNormal;
out vec4 FragColor;

void main()
{
    FragColor = vec4(normalize(vNormal) * 0.5 + 0.5, 1.0);
}
###FRAG/###
)";
}

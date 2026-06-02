#pragma once
#include <string>
#include <cstdint>
#include <utility>

namespace Twisted
{
	inline const std::string DebugUVShaderName = "debug_uvs_shader";
	inline constexpr std::pair<uint64_t, uint64_t> DebugUVShaderUUID{ 0xE3A79B42D1F08C67ULL, 0x9C4E12F7A6B3D550ULL };

	inline const std::string DebugUVShader =
R"(###VERT###
#version 430 core
#import "MVP_Core_Vert"

void main()
{
    vUV = aUV;
    gl_Position = TransformMVP(aPos);
}
###VERT/###

###FRAG###
#version 430 core
layout(location = 0) in vec2 vUV;
layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(vUV, 0.0, 1.0);
}
###FRAG/###
)";
}

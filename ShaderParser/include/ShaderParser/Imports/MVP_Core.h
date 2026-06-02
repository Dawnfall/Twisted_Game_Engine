#pragma once
#include <string>

namespace Twisted
{
	inline const std::string MVP_Core_Vert =
R"(layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

layout(push_constant) uniform PushConstants
{
    mat4 uModel;
    vec4 uColor;
};

layout(std140, binding = 0) uniform ShaderMVPBuffer
{
    mat4 uView;
    mat4 uProj;
    vec4 uCameraPos;
};

layout(location = 0) out vec2 vUV;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec3 vFragPos;

vec4 TransformMVP(vec3 point)
{
    return uProj * uView * uModel * vec4(point, 1.0);
}

vec3 GetWorldPos(vec3 localPos)
{
    return vec3(uModel * vec4(localPos, 1.0));
}

mat3 GetNormalMatrix()
{
    return transpose(inverse(mat3(uModel)));
}
)";

	inline const std::string MVP_Core_Frag =
R"(layout(push_constant) uniform PushConstants
{
    layout(offset = 64) vec4 uColor;
};

layout(std140, binding = 0) uniform ShaderMVPBuffer
{
    mat4 uView;
    mat4 uProj;
    vec4 uCameraPos;
};

layout(location = 0) in vec2 vUV;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vFragPos;
)";
}

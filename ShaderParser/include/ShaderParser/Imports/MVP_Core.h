#pragma once
#include <string>

namespace Twisted
{
	inline const std::string MVP_Core_Vert =
R"(layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

layout(std140, binding = 0) uniform ShaderMVPBuffer
{
    mat4 uModel;
    mat4 uView;
    mat4 uProj;
    vec4 uCameraPos; // xyz = world position, w unused
};

out vec2 vUV;
out vec3 vNormal;
out vec3 vFragPos;

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
R"(layout(std140, binding = 0) uniform ShaderMVPBuffer
{
    mat4 uModel;
    mat4 uView;
    mat4 uProj;
    vec4 uCameraPos; // xyz = world position, w unused
};

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;
)";
}

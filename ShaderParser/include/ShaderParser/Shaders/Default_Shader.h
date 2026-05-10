#pragma once
#include <string>
#include <cstdint>
#include <utility>

namespace Twisted
{
	inline const std::string DefaultShaderName = "Default Shader";
	inline constexpr std::pair<uint64_t, uint64_t> DefaultShaderUUID{ 0xFBC1E1A05F224861ULL, 0x83D7EF4512F97A55ULL };

	inline const std::string DefaultShader =
R"(###VERT###
#version 430 core
#import "MVP_Core_Vert"

void main()
{
    vUV      = aUV;
    vNormal  = GetNormalMatrix() * aNormal;
    vFragPos = GetWorldPos(aPos);
    gl_Position = TransformMVP(aPos);
}
###VERT/###

###FRAG###
#version 430 core
#import "MVP_Core_Frag"
#import "Lights_Core_Frag"

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4      uColor;

void main()
{
    vec3 albedo   = texture(uTexture, vUV).rgb * uColor.rgb;
    vec3 lighting = calcLighting(vFragPos, normalize(vNormal), uCameraPos.xyz, 32.0);
    FragColor     = vec4(lighting * albedo, uColor.a);
}
###FRAG/###

###REF###
texture uTexture Texture
color   uColor   Color
###REF/###
)";
}

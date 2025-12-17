#pragma once

#include "Twisted/Rendering/Shader.h"
#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include <string>

namespace Twisted::Collections
{
	const inline std::string lightShaderName = "Light Shader";
	constexpr AssetUuid lightShaderUuid{ 0x8F2C4D81A3E17B54ULL, 0x51D9F2C48BE0A19FULL };
	inline const std::string lightVertexShader =

		R"(
	#version 430 core
	
	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aNormal; 
	layout(location = 2) in vec2 aUV; 
	
	layout(std140) uniform MVP_uniforms
	{
		mat4 uModel;
		mat4 uView;
		mat4 uProj;
	};

	out vec2 vUV;
	
	void main()
	{
	    vUV = aUV;
	    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
	}
	)";

	inline const std::string lightFragmentShader =

		R"(
	#version 430 core
	
	in vec2 vUV;
	out vec4 FragColor;
	
	uniform sampler2D uTexture;
	uniform vec4 uColor;

	struct DirLight
	{
		float4 dir;
		float4 color;
		float intensity;
		float _padding[3];
	};

	void main()
	{
	    FragColor = texture(uTexture, vUV) * uColor;
	}
	)";

	void ImportLightsShader()
	{
		//AssetsService::GetInstance()->AddBuiltIn(Collections::simpleShaderUUID, simpleShader);
	}

}
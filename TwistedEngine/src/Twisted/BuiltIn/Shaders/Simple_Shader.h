#pragma once
#include "Twisted/Rendering/Shader.h"
#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/AssetsLayer/AssetsService.h"

#include <string>

namespace Twisted::Collections
{

	const inline std::string simpleShaderName = "Simple Shader";
	constexpr AssetUuid simpleShaderUUID{ 0xC1E53B529AB045D1ULL, 0xB9E45DE3A13B3E4CULL };
	inline const std::string simpleVertexShader =

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
	
	vec4 TransformMVPPoint(vec3 point, mat4 M, mat4 V, mat4 P)
	{
	    return P * V * M * vec4(point, 1.0);
	}	

	out vec2 vUV;
	
	void main()
	{
	    vUV = aUV;
	    gl_Position = TransformMVPPoint(aPos, uModel, uView, uProj);
	}
	)";

	inline const std::string simpleFragmentShader =

		R"(
	#version 430 core
	
	in vec2 vUV;
	out vec4 FragColor;
	
	uniform sampler2D uTexture;
	uniform vec4 uColor;
	
	void main()
	{
	    FragColor = texture(uTexture, vUV) * uColor;
	}
	)";

	void ImportSimpleShader()
	{
		Shader* simpleShader = TObject::Create<Shader>(Collections::simpleShaderName);
		ShaderData simpleShaderData;
		simpleShaderData.VertShader = Collections::simpleVertexShader;
		simpleShaderData.FragShader = Collections::simpleFragmentShader;
		Shader_GL::SetData(*simpleShader, simpleShaderData);

		AssetsService::GetInstance()->AddBuiltIn(Collections::simpleShaderUUID, simpleShader);
	}
}
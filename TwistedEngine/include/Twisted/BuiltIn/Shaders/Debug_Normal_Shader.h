#pragma once

#include "Twisted/Rendering/Shader.h"

#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include <string>

namespace Twisted::Collections
{

	inline const std::string debugNormalsShaderName = "debug_normals_shader";
	constexpr AssetUuid debugNormalsShaderUUID{ 0x7AB3E1D4C298F0EEULL, 0x44D1A99C08F3C2B1ULL };
	inline const std::string debugNormalsVertexShader =

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
	
	out vec3 normal;
	void main()
	{
		normal = aNormal;
	    gl_Position = TransformMVPPoint(aPos, uModel, uView, uProj);
	}
	)";

	inline const std::string debugNormalsFragmentShader =
		R"(
	#version 430 core
	in vec3 normal;
	out vec4 FragColor;
	void main()
	{
		vec3 coloredNormal = normal * 0.5 + 0.5;
		//FragColor = vec4(coloredNormal, 1.0);
		FragColor=vec4(1,1,1,1);
	}
	)";

	void ImportDebugNormalsShader()
	{
		Shader* debugNormalsShader = TObject::Create<Shader>(Collections::debugNormalsShaderName);
		ShaderData debugNormalsShaderData;
		debugNormalsShaderData.VertShader = Collections::debugNormalsVertexShader;
		debugNormalsShaderData.FragShader = Collections::debugNormalsFragmentShader;
		debugNormalsShader->SetData(debugNormalsShaderData);

		AssetsService::GetInstance()->AddBuiltIn(Collections::debugNormalsShaderUUID, debugNormalsShader);
	}
}
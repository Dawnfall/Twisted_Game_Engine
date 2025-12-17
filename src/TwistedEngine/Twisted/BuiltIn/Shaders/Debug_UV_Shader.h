#pragma once

#include "Twisted/Rendering/Shader.h"
#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include <string>

namespace Twisted::Collections
{
	inline const std::string debugUVsShaderName = "debug_uvs shader";
	constexpr AssetUuid debugUVsShaderUUID{ 0xE3A79B42D1F08C67ULL, 0x9C4E12F7A6B3D550ULL };
	inline const std::string debugUVsVertexShader =

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
	
	out vec2 uv;
	
	void main()
	{
	    uv = aUV;
	    //gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
	    gl_Position = vec4(aPos, 1.0);	
	}
	)";

	inline const std::string debugUVsFragmentShader =

		R"(
	#version 430 core
	in vec2 uv;
	out vec4 FragColor;
	void main()
	{
		FragColor = vec4(uv.x, uv.y, 0.0, 1.0); // green
	}
	)";

	void ImportDebugUVsShader()
	{
		Shader* debugUVsShader = TObject::Create<Shader>(Collections::debugUVsShaderName);
		ShaderData debugUVsShaderData;
		debugUVsShaderData.VertShader = Collections::debugUVsVertexShader;
		debugUVsShaderData.FragShader = Collections::debugUVsFragmentShader;
		Shader_GL::SetData(*debugUVsShader, debugUVsShaderData);

		AssetsService::GetInstance()->AddBuiltIn(Collections::debugUVsShaderUUID, debugUVsShader);

	}
}
#pragma once

#include "Twisted/Rendering/Shader.h"
#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include <string>

namespace Twisted::Collections
{
	inline const std::string defaultShaderName = "Default Shader";
	constexpr AssetUuid defaultShaderUUID{ 0xFBC1E1A05F224861ULL, 0x83D7EF4512F97A55ULL };
	inline const std::string defaultVertexShader =

		R"(
	#version 430 core
	layout(location = 0) in vec3 aPos;
	void main()
	{
		gl_Position = vec4(aPos, 1.0);
	}
	)";

	inline const std::string defaultFragmentShader =

		R"(
	#version 430 core
	out vec4 FragColor;
	void main()
	{
		FragColor = vec4(0.0, 1.0, 0.0, 1.0); // green
	}
	)";

	void ImportDefaultShader()
	{
		Shader* defaultShader = TObject::Create<Shader>(Collections::defaultShaderName);
		ShaderData defaultShaderData;
		defaultShaderData.VertShader = Collections::defaultVertexShader;
		defaultShaderData.FragShader = Collections::defaultFragmentShader;
		Shader_GL::SetData(*defaultShader, defaultShaderData);

		AssetsService::GetInstance()->AddBuiltIn(Collections::defaultShaderUUID, defaultShader);

	}
}


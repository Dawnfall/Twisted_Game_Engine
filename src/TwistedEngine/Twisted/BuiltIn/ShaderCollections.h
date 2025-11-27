#pragma once

#include <string>

namespace Twisted::Collections
{
	//**************
	// Debug shaders
	//**************

	//************ normals

	inline const std::string debugNormalsShaderName = "debug_normals shader";
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
	
	out vec2 vUV;
	out vec3 normal;
	
	void main()
	{
	    vUV = aUV;
	    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
		normal = aNormal;
	}
	)";

	inline const std::string debugNormalsFragmentShader =
		R"(
	#version 430 core
	in vec3 normal;
	out vec4 FragColor;
	void main()
	{
		FragColor = vec4(normal.x, normal.y, 0.0, 1.0); // green
	}
	)";

	//********* uvs

	inline const std::string debugUVsShaderName = "debug_uvs shader";
	constexpr AssetUuid debugUVsShaderUUID { 0xE3A79B42D1F08C67ULL, 0x9C4E12F7A6B3D550ULL };
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
	
	out vec2 vUV;
	out vec2 uv;
	
	void main()
	{
	    vUV = aUV;
	    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
		uv=aUV;	
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
	//**************
	// Default shader
	//**************
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
	
	out vec2 vUV;
	
	void main()
	{
	    vUV = aUV;
	    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
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

	//*************
	// Light Shader
	//*************
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

}

/* UUIDs
{ 0xF12D8C33B7A4509AULL, 0x62A4E9D0C8B15F23ULL }
{ 0x9D77A4E1C3395B20ULL, 0xFEB218D4A6C7093DULL }
{ 0x4B98F6C2D1157A9FULL, 0x83D44AA129E0FE31ULL }
{ 0xC8E2B17495AF03D2ULL, 0x1D76C5E39AA4E870ULL }
{ 0xA1F4DE62B38C9045ULL, 0x7F23C8B1D4E96A8CULL }
{ 0x3CE7A1B59D0F72A4ULL, 0xE4B99873CDA25119ULL }
{ 0xD2A4587CB90E13EFULL, 0x6AFC0471E8B3D95CULL }
*/
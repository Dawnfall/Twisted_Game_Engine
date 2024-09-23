#pragma once

#include "pch.h"

namespace Twisted::Collections
{
	const std::string defaultShaderName = "defaultShader";
	const std::string simpleVertexCode=
		R"(
#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0f);
		TexCoord = aTexCoord;
	}
)";

	const std::string simpleFragmentCode=
		R"(
#version 410 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord); 
} 
)";

	const std::string vertexShaderCode=
		R"(
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}
)";

	const std::string fragmentShaderCode=
		R"(
#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

uniform vec4 test_color; 
void main()
{
    FragColor = test_color;//vec4(1.0,1.0,1.0,1.0); //vertexColor;
} 
)";
}
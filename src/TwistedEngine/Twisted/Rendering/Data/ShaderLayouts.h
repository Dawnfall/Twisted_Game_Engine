#pragma once
#include "Utils\GlmUtils.h"

namespace Twisted
{
	struct ShaderMVPBuffer
	{
		Mat4x4f ModelMatrix;
		Mat4x4f ViewMatrix;
		Mat4x4f ProjectionMatrix;
	};

	struct ShaderLight
	{
		int type; 
		Vec4f color;
		float intensity;
		Vec3f position; 
		float range; 
		Vec3f direction; 
		float innerCos;  
		float outerCos;
	};
}

/*
layout(std140) uniform ShaderMVPBuffer
{
	mat4 uModel;
	mat4 uView;
	mat4 uProj;
};

layout(std140) uniform LightsBuffer
{
	ShaderLight lights[128];   // max number of lights
	int lightCount;
};
*/
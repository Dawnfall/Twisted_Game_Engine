#pragma once
#include "Utils\GlmUtils.h"

namespace Twisted
{
	struct ShaderDataLayout
	{
		Mat4x4f ModelMatrix;
		Mat4x4f ProjectionMatrix;
		Mat4x4f ViewMatrix;
	};
}

/*
layout(std140) uniform Matrices
{
	mat4 uModel;
	mat4 uView;
	mat4 uProj;
};
*/
#pragma once

#include "pch.h"
#include "Resource.h"

namespace Twisted
{
	struct ShaderData :Resource
	{
		std::string VertShaderCode = "";
		std::string FragShaderCode = "";
	};
}
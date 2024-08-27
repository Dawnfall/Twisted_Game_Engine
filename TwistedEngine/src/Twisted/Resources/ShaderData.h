#pragma once

#include "Resource.h"
#include <string>
namespace Twisted
{
	struct ShaderData :Resource
	{
		std::string VertShaderCode = "";
		std::string FragShaderCode = "";
	};
}
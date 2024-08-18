#pragma once

#include "Resource.h"
#include <string>
namespace Dawn
{
	struct ShaderData :Resource
	{
		std::string VertShaderCode = "";
		std::string FragShaderCode = "";
	};
}
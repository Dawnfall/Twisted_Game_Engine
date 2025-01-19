#pragma once
#include "AppCore.h"
#include <string>

namespace Twisted
{
	struct ShaderData
	{
		std::string Name = "";
		std::string VertShaderCode = "";
		std::string FragShaderCode = "";
	};
}
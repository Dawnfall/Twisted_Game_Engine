#pragma once

#include <string>

namespace Twisted
{
	struct ShaderData
	{
		std::string Name = "";
		std::string VertShaderCode = "";
		std::string FragShaderCode = "";

		bool IsEmpty() { return VertShaderCode == ""; }
	};
}
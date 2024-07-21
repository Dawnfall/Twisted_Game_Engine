#pragma once

#include "Resource.h"
#include <string>
namespace Dawn
{
	struct ShaderData :Resource
	{
		ShaderData(const std::string& name, const std::string& vertShaderCode = "", const std::string& fragShaderCode = "") :
			Resource(name),
			VertShaderCode(vertShaderCode),
			FragShaderCode(fragShaderCode)
		{}

		std::string VertShaderCode = "";
		std::string FragShaderCode = "";
	};
}
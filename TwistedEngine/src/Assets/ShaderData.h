#pragma once
#include "AppCore.h"
#include "Asset.h"

namespace Twisted
{
	struct TWISTED_API ShaderData :public Asset
	{
		std::string Name = "";
		std::string VertShaderCode = "";
		std::string FragShaderCode = "";
	};
}
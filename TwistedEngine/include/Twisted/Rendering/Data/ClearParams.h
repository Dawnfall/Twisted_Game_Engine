#pragma once

#include "Utils/GlmUtils.h"

namespace Twisted
{
	struct ClearParams
	{
		bool doDepthClear = true;
		bool doClearStencil = true;
		bool doClearColor = true;

		Vec4f clearColor{ 0,0,0,1 };
	};
}
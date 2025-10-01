#pragma once

#include "Twisted/Rendering/RenderContext.h"
#include "Twisted/Gameing/Components/CCamera.h"

namespace Twisted
{
	struct RenderSystemEntry
	{
		RenderContext* Context;
		CCamera* Camera;
	};
}
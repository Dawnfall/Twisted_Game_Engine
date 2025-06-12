#pragma once

#include "Twisted/Rendering/RenderContext.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/Entity.h"

namespace Twisted
{
	struct RenderSystemEntry
	{
		Twisted::Render::RenderContext* Context;
		EntityID Camera;
	};
}
#pragma once

#include "Rendering/RenderContext.h"
#include "Game/Components/CCamera.h"
#include "Game/Entity.h"

namespace Twisted
{
	struct RenderSystemEntry
	{
		Twisted::Render::RenderContext* Context;
		EntityID Camera;
	};
}
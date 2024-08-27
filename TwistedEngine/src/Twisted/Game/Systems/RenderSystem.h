#pragma once

#include "Twisted/Game/SystemBase.h"
#include "Twisted/Application.h"
#include "Twisted/GameCore.h"
#include "Twisted/Game/Components/CRenderer.h"

namespace Twisted
{
	class RenderSystem :public SystemBase
	{
		void Update(Application& app) override;
	};
}
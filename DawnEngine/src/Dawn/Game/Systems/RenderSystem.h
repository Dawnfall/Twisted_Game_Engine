#pragma once

#include "Dawn/Game/SystemBase.h"
#include "Dawn/Application.h"
#include "Dawn/GameCore.h"
#include "Dawn/Game/Components/CRenderer.h"

namespace Dawn
{
	class RenderSystem :public SystemBase
	{
		void Update(Application& app) override;
	};
}
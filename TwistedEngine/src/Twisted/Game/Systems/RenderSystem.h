#pragma once

#include "AppCore.h"
#include "Twisted/Game/SystemBase.h"

namespace Twisted
{
	class TWISTED_API RenderSystem :public SystemBase
	{
		void Update(AppBase* app, World* world) override;
	};
}
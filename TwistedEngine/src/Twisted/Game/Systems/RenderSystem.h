#pragma once

#include "Twisted/Game/SystemBase.h"


namespace Twisted
{
	class RenderSystem :public SystemBase
	{
		void Update(Application* app) override;
	};
}
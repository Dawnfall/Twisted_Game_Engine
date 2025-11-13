#pragma once

#include "UI/ComponentPainter.h"
#include "Twisted/Gameing/Components/CLight.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"

namespace Twisted::Editor
{
	class CLightPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;
	};
}

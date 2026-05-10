#pragma once

#include "UI/ComponentPainter.h"
#include "EditorApp/EditorRegistry.h"
#include "Components/SpotLightComponent.h"

namespace Twisted::Editor
{
	class SpotLightPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;
	};
}

REGISTER_COMPONENT_PAINTER(SpotLightPainter, SpotLightComponent)

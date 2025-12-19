#pragma once

#include "UI/ComponentPainter.h"
#include "EditorApp/EditorRegistry.h"
#include "Twisted/Gameing/Components/DirectionalLightComponent.h"

namespace Twisted::Editor
{
	class DirectionalLightPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;
	};
}

REGISTER_COMPONENT_PAINTER(DirectionalLightPainter,DirectionalLightComponent)

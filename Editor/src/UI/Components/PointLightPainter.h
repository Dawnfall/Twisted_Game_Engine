#pragma once

#include "UI/ComponentPainter.h"
#include "EditorApp/EditorRegistry.h"
#include "Twisted/Gameing/Components/PointLightComponent.h"

namespace Twisted::Editor
{
	class PointLightPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;
	};
}

REGISTER_COMPONENT_PAINTER(PointLightPainter, PointLightComponent)

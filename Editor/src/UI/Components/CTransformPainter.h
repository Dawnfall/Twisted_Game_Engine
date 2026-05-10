#pragma once

#include "UI/ComponentPainter.h"
#include "Components/CTransform.h"
#include "World.h"

#include "imgui.h"

namespace Twisted::Editor
{
	class CTransformPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj) override;
	};
}



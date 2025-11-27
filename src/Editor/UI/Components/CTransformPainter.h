#pragma once

#include "UI/ComponentPainter.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/World.h"

#include "imgui.h"

namespace Twisted::Editor
{
	class CTransformPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj) override;
	};
}



#pragma once

#include "UI/ComponentPainter.h"
#include "Twisted/Gameing/Components/CRenderer.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"

namespace Twisted::Editor
{
	class CRendererPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;
	};
}

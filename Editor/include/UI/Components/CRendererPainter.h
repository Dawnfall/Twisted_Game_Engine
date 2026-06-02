#pragma once

#include "UI/ComponentPainter.h"
#include "Components/CRenderer.h"
#include "UI/ImguiExtensions.h"
#include "World.h"

namespace Twisted::Editor
{
	class CRendererPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;
	};
}

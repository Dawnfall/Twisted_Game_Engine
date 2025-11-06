#pragma once

#include "UI/ComponentPainter.h"

namespace Twisted::Editor
{
	class CCameraPainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;
	};
}
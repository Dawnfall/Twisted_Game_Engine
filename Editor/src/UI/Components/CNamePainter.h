#pragma once

#include "UI/ComponentPainter.h"
#include "Components/CName.h"
#include "UI/ImguiExtensions.h"
#include "World.h"

namespace Twisted::Editor
{
	class CNamePainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;

	};
}

#pragma once

#include "UI/ComponentPainter.h"
#include "Twisted/Gameing/Components/CName.h"
#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/World.h"

namespace Twisted::Editor
{
	class CNamePainter :public ComponentPainter
	{
	public:
		void Paint(void* obj)override;

	};
}
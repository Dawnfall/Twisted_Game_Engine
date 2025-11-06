#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/Entity.h"

namespace Twisted
{
	class World;
}

namespace Twisted::Editor
{
	class ComponentPainter
	{
	public:
		virtual void Paint(void* obj) = 0;
	};
}
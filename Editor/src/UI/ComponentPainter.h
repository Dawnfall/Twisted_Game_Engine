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
		virtual void AddComponent(Entity entity) = 0;
		virtual void* GetComponent(Entity entity) = 0;
		virtual void Paint(void* obj) = 0;
		virtual std::string GetComponentName()const = 0;
	};
}
#include "AppCore.h"

#pragma once

namespace Twisted
{
	class World;
	class TWISTED_API ManagerBase
	{
	public:
		ManagerBase(World* world) :
			world(world)
		{
		}

		World* world;
	private:
	};
}
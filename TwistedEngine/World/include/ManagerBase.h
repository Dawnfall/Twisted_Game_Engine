#pragma once

#include "AppCore.h"

namespace Twisted
{
	class World;
	class TWISTED_API ManagerBase
	{
	public:
		ManagerBase(World* w) :
			world(w)
		{
		}

		World* world;
	private:
	};
}

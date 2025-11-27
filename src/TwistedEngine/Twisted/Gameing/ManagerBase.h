#pragma once

namespace Twisted
{
	class World;
	class ManagerBase
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
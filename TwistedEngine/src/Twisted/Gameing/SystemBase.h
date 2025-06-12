#pragma once

#include "AppCore.h"

namespace Twisted
{
	class World;
	class TWISTED_API SystemBase
	{
	public:
		SystemBase(World* world):m_world(world){}
		virtual ~SystemBase() = default;
		virtual void Update() = 0;

	protected:
		World* m_world;
	};
}
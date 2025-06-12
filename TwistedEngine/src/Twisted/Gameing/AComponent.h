#pragma once

#include "AppCore.h"
#include "Entity.h"

namespace Twisted
{
	class World;
	class TWISTED_API AComponent
	{
	public:
		AComponent(EntityID entityID, World* world) :
			m_entityID(entityID),
			m_world(world)
		{
		}

		const EntityID GetEntityID()const 
		{
			return m_entityID; 
		}
	protected:
		EntityID m_entityID;
		World* m_world;
	};
}

#pragma once

#include "EnTT/entt.hpp"

#include "Dawn/Game/World.h"

namespace Dawn
{
	class World;

	class AComponent
	{
	public:
		AComponent(entt::entity parentEntity, World* world) :
			m_entityID(parentEntity),
			m_world(world)
		{}
	protected:
		entt::entity m_entityID;
		World* m_world;
	};
}

#include "World.h"

#include "EnTT/entt.hpp"

#include "Dawn/Game/Components/Transform.h"

namespace Dawn
{
	entt::entity World::CreateEntity()
	{
		entt::entity entityID = m_registry.create();
		AddComponent<Transform>(entityID);

		return entityID;
	}

	entt::registry& World::GetRegistry()
	{
		return m_registry;
	}
}
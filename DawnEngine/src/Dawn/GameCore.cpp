#include "GameCore.h"

#include "EnTT/entt.hpp"
#include "Dawn/Game/Components/Transform.h"

namespace Dawn
{
	entt::entity GameCore::CreateEntity()
	{
		entt::entity entityID = m_registry.create();
		//AddComponent<Transform>(entityID);

		return entityID;
	}

	entt::registry& GameCore::GetRegistry()
	{
		return m_registry;
	}
}
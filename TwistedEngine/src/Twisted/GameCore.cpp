#include "GameCore.h"

#include "EnTT/entt.hpp"
#include "Twisted/Game/Components/CTransform.h"

namespace Twisted
{
	entt::entity GameCore::CreateEntity()
	{
		entt::entity entityID = m_registry.create();
		//AddComponent<Transform>(entityID);

		return entityID;
	}
	std::vector<entt::entity> GameCore::CreateEntities(int amount)
	{
		std::vector<entt::entity> res(amount);
		m_registry.create(res.begin(), res.end());
		return res;
	}

	void GameCore::DestroyEntity(entt::entity id)
	{
		m_registry.destroy(id);
	}
	void GameCore::DestroyEntities(const std::vector<entt::entity>& entities)
	{
		auto it1 = entities.begin();
		m_registry.destroy(entities.begin(), entities.end());
	}

}
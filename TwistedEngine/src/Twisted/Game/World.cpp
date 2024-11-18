#include "twistedpch.h"
#include "World.h"
#include "SystemBase.h"

#include "Twisted/Game/Components/CTransform.h"

namespace Twisted
{
	void World::UpdateFrame(AppBase* app, TimeManager& time)
	{
		time.UpdateNewFrame();

		for (auto& system : m_systems)
			system->Update(app);
	}

	EntityID World::CreateNewEntity(EntityID id)
	{
		EntityID entityID = NullEntity;
		if (id != NullEntity)
			entityID = m_registry.create(id);
		else
			entityID = m_registry.create();

		m_registry.emplace<CTransform>(entityID, entityID, this);

		return entityID;
	}

}
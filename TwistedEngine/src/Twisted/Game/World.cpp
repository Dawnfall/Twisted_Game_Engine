#include "twistedpch.h"
#include "World.h"
#include "SystemBase.h"

#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Systems/RenderSystem.h"

namespace Twisted
{
	World::World()
	{
		m_systems.emplace_back(std::make_unique<RenderSystem>());
	}

	void World::UpdateFrame(AppBase* app)
	{
		for (auto& system : m_systems)
			system->Update(app,this);
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
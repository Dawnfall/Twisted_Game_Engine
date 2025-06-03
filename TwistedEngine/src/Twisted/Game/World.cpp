#include "twistedpch.h"
#include "World.h"
#include "SystemBase.h"

#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CName.h"
#include "Twisted/Game/Systems/RenderSystem.h"

namespace Twisted
{
	World::World(Application* app) :
		m_app(app)
	{
	}

	void World::UpdateFrame()
	{
		for (auto& system : m_systems)
			system->Update();
	}

	EntityID World::CreateNewEntity(EntityID id)
	{
		EntityID entityID = NullEntity;
		if (id != NullEntity)
			entityID = m_registry.create(id);
		else
			entityID = m_registry.create();

		m_registry.emplace<CTransform>(entityID, entityID, this);
		m_registry.emplace<CName>(entityID, entityID, this);
		return entityID;
	}

	void World::Serialize(BinSerializer& buffer)const
	{
		std::vector<EntityID> allEntityIDs = GetAllEntityIDs();
		buffer.Write<std::vector<EntityID>>(allEntityIDs);

		serializeComponents<CTransform>(buffer);
		serializeComponents<CName>(buffer);
	}

	void World::Deserialize(BinSerializer& buffer)
	{
		std::vector<EntityID> allEntityIDs = buffer.Read<std::vector<EntityID>>();
		for (EntityID id : allEntityIDs)
			m_registry.create(id);

		deSerializeComponents<CTransform>(buffer);
		deSerializeComponents<CName>(buffer);
	}
}
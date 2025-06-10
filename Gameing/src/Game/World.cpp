#include "World.h"
#include "SystemBase.h"

#include "Game/Components/CTransform.h"
#include "Game/Components/CName.h"
#include "Game/Systems/RenderSystem.h"

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

	void World::Serialize(BinSerializer& buffer)const
	{
		buffer.Write<std::string>(GetUUID());

		std::vector<EntityID> allEntityIDs = GetAllEntityIDs();
		buffer.Write<std::vector<EntityID>>(allEntityIDs);

		buffer.Write<std::vector<EntityID>>(m_rootEntities);

		serializeComponents<CTransform>(buffer);
		serializeComponents<CName>(buffer);
	}

	void World::Deserialize(BinSerializer& buffer)
	{
		m_uuid = buffer.Read<std::string>();

		std::vector<EntityID> allEntityIDs = buffer.Read<std::vector<EntityID>>();
		m_registry.create(allEntityIDs.begin(), allEntityIDs.end());

		m_rootEntities = buffer.Read<std::vector<EntityID>>();

		deSerializeComponents<CTransform>(buffer);
		deSerializeComponents<CName>(buffer);
	}
}
#include "twistedpch.h"
#include "WorldSerializer.h"
#include "Twisted/Serialization/SerializationBuffer.h"

#include "Twisted/Game/World.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CCamera.h"
#include "Twisted/Game/Components/CRenderer.h"
#include "Twisted/Game/Components/CLight.h"

namespace Twisted
{
	void WorldSerializer::SerializeWorld(const World* world)
	{
		SerializationBuffer buffer;

		//entities
		std::vector<EntityID> allEntityIDs = world->GetAllEntityIDs();
		buffer.WriteVec<EntityID>(allEntityIDs);

		//components
		auto serializeComponents = [&buffer, &world]<typename T>() {
			auto componentsView = world->GetComponents<T>();
			buffer.Write<size_t>(componentsView.size());
			for (auto entId : componentsView)
			{
				const T component = componentsView.get<0>(entId);
				buffer.Write<EntityID>(component.GetEntityID());
				Serialize<T>(component, buffer);
			}
		};

		serializeComponents.operator() < CTransform > ();
		serializeComponents.operator() < CRenderer > ();
		serializeComponents.operator() < CCamera > ();
		serializeComponents.operator() < CLight > ();
	}

	std::shared_ptr<World> WorldSerializer::DeserializeWorld(SerializationBuffer& buffer)
	{
		std::shared_ptr<World> world = std::make_shared<World>();

		//entities
		std::vector<EntityID> allEntityIDs = buffer.ReadVec<EntityID>();
		for (EntityID id : allEntityIDs)
			world->CreateEntity(id);

		//components
		auto deserializeComponents = [&buffer, &world]<typename T>() {

			size_t componentSize = buffer.Read<size_t>();
			for (int i = 0; i < componentSize; i++)
			{
				EntityID entityID = buffer.Read<EntityID>();
				T component = world->m_registry.emplace<T>(entityID, entityID, world.get());
				Deserialize<T>(component, buffer);
			}
		};

		deserializeComponents.operator() < CTransform > ();
		deserializeComponents.operator() < CRenderer > ();
		deserializeComponents.operator() < CCamera > ();
		deserializeComponents.operator() < CLight > ();

		return world;
	}

}

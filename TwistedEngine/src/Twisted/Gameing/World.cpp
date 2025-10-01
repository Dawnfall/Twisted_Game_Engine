#include "World.h"
#include "SystemBase.h"

#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"
#include "Twisted/Gameing/Systems/RenderSystem.h"

#include "ComponentRegistry.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"

namespace Twisted
{
	World::World(ObjectID id) :
		BaseObject(id)
	{
	}

	void World::Clear()
	{
		m_registry.clear();
		m_systems.clear();
	}

	void World::DestroyEntity(EntityID entity)
	{
		if (m_registry.valid(entity))
			m_registry.destroy(entity);
	}

	void World::UpdateFrame()
	{
		for (auto& system : m_systems)
			system->Update();
	}

	BinSerializer World::Serialize(AssetsLayer* assetsLayer)const
	{
		Twisted::BinSerializer buffer;

		buffer.Write<std::vector<EntityID>>(GetRootEntities());
		for_each_type(AllComponents{}, [&buffer, this, assetsLayer]<typename T>() {
			auto view = m_registry.view<T>();

			// Count the elements manually
			size_t count = 0;
			for (auto entity : view) ++count;
			buffer.Write<size_t>(count);

			for (auto entity : view)
			{
				buffer.Write<EntityID>(entity);
				view.get<T>(entity).Serialize(buffer, assetsLayer);
			}
		});

		return buffer;
	}

	void World::Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)
	{
		m_rootEntities = buffer.Read<std::vector<EntityID>>();

		for_each_type(AllComponents{}, [&buffer, this, assetsLayer]<typename T>() {

			size_t compCount = buffer.Read<size_t>();
			for (int i = 0; i < compCount; i++)
			{
				EntityID entID = buffer.Read<EntityID>();
				if (!m_registry.valid(entID))
					m_registry.create(entID);

				T& newComponent = m_registry.emplace<T>(entID, entID, this);
				newComponent.Deserialize(buffer, assetsLayer);
			}
		});
	}

	/*void World::PostSerialize(BinSerializer& buffer)const
	{
		forEachType<AllComponents>([&]<typename T>() {

			auto view = m_registry.view<T>();

			size_t count = 0;
			for (auto entity : view) ++count;
			buffer.Write<size_t>(count);

			for (auto entity : view)
			{
				buffer.Write<EntityID>(entity);
				view.get<T>(entity).PostSerialize(buffer);
			}
		});
	}

	void World::PostDeserialize(BinSerializer& buffer)
	{
		forEachType<AllComponents>([&]<typename T>() {

			size_t compCount = buffer.Read<size_t>();
			for (int i = 0; i < compCount; i++)
			{
				EntityID entID = buffer.Read<EntityID>();
				m_registry.get<T>(entID).PostDeserialize(buffer);
			}
		});
	}*/
}
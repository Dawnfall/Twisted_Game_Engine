#pragma once

#include "AppCore.h"
#include "Twisted/RegisterLayer/Serialization/BinSerializer.h"


namespace Twisted
{
	class World;
	class AssetsLayer;

	class TWISTED_API AComponent
	{
	public:
		AComponent(EntityID entity,World* world) :
			m_entityID(entity),
			m_world(world)
		{
		}

		const EntityID GetEntityID()const { return m_entityID; }
		World* GetWorld() { return m_world; }
		const World* GetWorld()const { return m_world; }

		virtual void Init(){}

		virtual void Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer) const {};
		virtual void Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer) {};

		//virtual void PostSerialize(BinSerializer& buffer)const{}
		//virtual void PostDeserialize(BinSerializer& buffer, AssetsLayer* assetsLayer) {}

	protected:

		EntityID m_entityID;
		World* m_world = nullptr;
	};
}



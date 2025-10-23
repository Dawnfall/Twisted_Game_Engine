#pragma once

#include "AppCore.h"
#include "Serialization/BinSerializer.h"

#include "Twisted/Gameing/Entity.h"

namespace Twisted
{
	class World;
	class TWISTED_API AComponent
	{
	public:
		AComponent(Entity entity) :
			m_entity(entity)
		{
		}

		const Entity& GetEntity()const { return m_entity; }

		virtual void Init() {}

		virtual void Serialize(BinSerializer& buffer) const {};
		virtual void Deserialize(BinSerializer& buffer) {};

		//virtual void PostSerialize(BinSerializer& buffer)const{}
		//virtual void PostDeserialize(BinSerializer& buffer, AssetsLayer* assetsLayer) {}

	protected:

		Entity m_entity;
	};
}



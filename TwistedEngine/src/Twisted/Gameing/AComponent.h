#pragma once

#include "AppCore.h"
#include "Serialization/BinSerializer.h"

#include "Twisted/Gameing/Entity.h"
#include "yaml-cpp/yaml.h"
#include "Debug/Logger.h"

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
		virtual ~AComponent()
		{
		}

		virtual void OnInit() {}
		virtual void OnDestroy() {}

		const Entity& GetEntity()const { return m_entity; }
		const World* GetWorld()const { return m_entity.GetWorld(); }
		const EntityID& GetID()const { return m_entity.GetID(); }

		World* GetWorld() { return m_entity.GetWorld(); }

		virtual void Serialize(BinSerializer& buffer) const {}
		virtual void Deserialize(BinSerializer& buffer) {}

		virtual YAML::Node YamlSerialize() const = 0;
		virtual void YamlDeserialize(const YAML::Node& node) = 0;

		//virtual void PostSerialize(BinSerializer& buffer)const{}
		//virtual void PostDeserialize(BinSerializer& buffer, AssetsLayer* assetsLayer) {}

	protected:

		Entity m_entity;
	};
}





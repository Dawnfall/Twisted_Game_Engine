#pragma once

#include "twistedpch.h"
#include "AppCore.h"
#include "Twisted/Game/AComponent.h"
#include "Twisted/Serialization/WorldSerializer.h"

namespace Twisted
{
	class Material;
	class Mesh;

	class TWISTED_API CRenderer :public AComponent
	{
	public:
		CRenderer(EntityID entityID, World* world) :AComponent(entityID, world) {}

		std::shared_ptr<Material> Material;
		std::shared_ptr<Mesh> Mesh;

	};

	template<>
	inline void Serialize(const CRenderer& renderer,SerializationBuffer& serializer)
	{
		
	}

	template<>
	inline void Deserialize(CRenderer& renderer,SerializationBuffer& serializer)
	{

	}
}
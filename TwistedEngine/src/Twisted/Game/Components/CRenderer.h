#pragma once

#include "AppCore.h"
#include "Twisted/Game/AComponent.h"
#include "Serialization/WorldSerializer.h"

namespace Twisted
{
	class Material;
	class Mesh;

	class TWISTED_API CRenderer :public AComponent
	{
	public:
		CRenderer(EntityID entityID, World* world) :AComponent(entityID, world) {}

		SRef<Material> Material;
		SRef<Mesh> Mesh;

	};

	template<>
	inline void Serialize(const CRenderer& renderer,BinSerializer& serializer)
	{
		
	}

	template<>
	inline void Deserialize(CRenderer& renderer,BinSerializer& serializer)
	{

	}
}
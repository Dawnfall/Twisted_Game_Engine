#pragma once

#include "AppCore.h"
#include "Game/AComponent.h"
#include "Game/Serialization/WorldSerializer.h"

namespace Twisted::Render
{
	class Material;
	class Mesh;
}

namespace Twisted
{
	class TWISTED_API CRenderer :public AComponent
	{
	public:
		CRenderer(EntityID entityID, World* world) :AComponent(entityID, world) {}

		SRef<Render::Material> Material;
		SRef<Render::Mesh> Mesh;

	};

	//template<>
	//inline void Serialize(const CRenderer& renderer,BinSerializer& serializer)
	//{
	//	
	//}

	//template<>
	//inline void Deserialize(CRenderer& renderer,BinSerializer& serializer)
	//{

	//}
}
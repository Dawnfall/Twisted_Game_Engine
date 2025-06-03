#pragma once

#include "AppCore.h"
#include "Twisted/Game/AComponent.h"
#include "Serialization/WorldSerializer.h"

namespace Twisted
{
	enum class TWISTED_API LightType
	{

	};

	class TWISTED_API CLight :public AComponent
	{
	public:
		CLight(EntityID entityID, World* world) :AComponent(entityID,world) {}

		LightType Type;
	};

	template<>
	inline void Serialize(const CLight& light, BinSerializer& serializer)
	{
		//serializer.Write<LightType>(light.Type);
	}

	template<>
	inline void Deserialize(CLight& light, BinSerializer& serializer)
	{
		//light.Type = serializer.Read<LightType>();
	}
}
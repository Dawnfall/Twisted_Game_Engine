#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"

namespace Twisted
{
	enum class TWISTED_API LightType
	{
		DIRECTIONAL = 0,
		POINT = 1
	};

	class TWISTED_API CLight :public AComponent
	{
	public:
		CLight(Entity entity) :
			AComponent(entity),
			m_type(LightType::DIRECTIONAL)
		{
		}

		LightType GetType()const { return m_type; }
		void SetType(LightType lightType) { m_type = lightType; }

		void Serialize(BinSerializer& buffer)const override;
		void Deserialize(BinSerializer& buffer)override;
	private:
		LightType m_type;
	};

	//template<>
	//inline void Serialize(const CLight& light, BinSerializer& serializer)
	//{
	//	serializer.Write<LightType>(light.Type);
	//}

	//template<>
	//inline void Deserialize(CLight& light, BinSerializer& serializer)
	//{
	//	light.Type = serializer.Read<LightType>();
	//}
}
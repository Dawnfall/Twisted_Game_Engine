#pragma once

#include "Twisted/Gameing/AComponent.h"
#include "Twisted/RegisterLayer/Serialization/BinSerializer.h"

namespace Twisted
{
	class TWISTED_API CName :public AComponent
	{
	public:
		CName(EntityID entity, World* world) :AComponent(entity,world),
			m_name(std::to_string(static_cast<int>(entity)))
		{}

		const std::string& GetName()const { return m_name; }
		void SetName(const std::string& newName) { m_name = newName; }

		void Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const override;
		void Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)override;

	private:
		std::string m_name;
	};

}
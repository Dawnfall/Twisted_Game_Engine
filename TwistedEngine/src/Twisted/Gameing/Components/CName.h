#pragma once

#include "Twisted/Gameing/AComponent.h"
#include "Serialization/BinSerializer.h"

namespace Twisted
{
	class TWISTED_API CName :public AComponent
	{
	public:
		CName(Entity entity) :AComponent(entity),
			m_name(std::to_string(static_cast<int>(m_entity.GetID())))
		{}

		const std::string& GetName()const { return m_name; }
		void SetName(const std::string& newName) { m_name = newName; }

		void Serialize(BinSerializer& buffer)const override;
		void Deserialize(BinSerializer& buffer)override;

	private:
		std::string m_name;
	};

}
#pragma once

#include "Twisted/Game/AComponent.h"
#include "Serialization/Serializer.h"

namespace Twisted
{
	class CName :public AComponent
	{
	public:
		CName(EntityID id, World* world) :AComponent(id, world),
			m_name(std::to_string(static_cast<int>(id)))
		{}

		const std::string& GetName()const { return m_name; }
		void SetName(const std::string& newName) { m_name = newName; }

		void Serialize(BinSerializer& buffer)const
		{
			buffer.Write<std::string>(m_name);
		}
		void Deserialize(BinSerializer& buffer)
		{
			m_name = buffer.Read<std::string>();
		}

	private:
		std::string m_name;
	};

}
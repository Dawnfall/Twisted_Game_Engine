#pragma once

#include "twistedpch.h"

namespace Twisted
{
	class AComponent
	{
	public:
		AComponent(entt::entity entityID) :
			m_entityID(entityID)
		{}

		const entt::entity& GetID()const { return m_entityID; }
	protected:
		entt::entity m_entityID;
	};
}

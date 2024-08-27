#pragma once

#include "EnTT/entt.hpp"


namespace Twisted
{
	struct AComponent
	{
		AComponent(entt::entity entityID) :
			m_entityID(entityID)
		{}

		int GetID() { return (int)m_entityID; }
	protected:
		entt::entity m_entityID;
	};
}

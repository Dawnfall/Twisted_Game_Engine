#pragma once

#include "EnTT/entt.hpp"

#include "Dawn/GameCore.h"

namespace Dawn
{
	class World;

	class AComponent
	{
	public:
		AComponent(GameCore* gameCore,entt::entity entityID) :
			m_gameCore(gameCore),
			m_entityID(entityID)
		{}

		
	protected:
		GameCore* m_gameCore;
		entt::entity m_entityID;
	};
}

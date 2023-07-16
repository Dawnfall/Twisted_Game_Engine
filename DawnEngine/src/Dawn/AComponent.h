#pragma once

#include "GameEntity.h"

namespace Dawn
{
	class GameEntity;

	class AComponent
	{
	public:
		AComponent(GameEntity* parentEntity) :
			m_gameEntity(parentEntity)
		{
			
		}
	protected:
		GameEntity* m_gameEntity;
	};
}

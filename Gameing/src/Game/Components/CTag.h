#pragma once
#include "Twisted/Game/AComponent.h"

namespace Twisted
{
	class CTag :public AComponent
	{
	public:
		CTag(EntityID id,World* world):AComponent(id,world){}
	};
}
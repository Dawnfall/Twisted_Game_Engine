#pragma once
#include "SystemBase.h"
#include "WorldRegistry.h"

namespace Twisted::Editor
{
	class PlayerSystem : public SystemBase
	{
		TCLASS_BODY()
	public:
		PlayerSystem(World* world) : SystemBase(world) {}

		void Update(float deltaTime) override;
	};
}


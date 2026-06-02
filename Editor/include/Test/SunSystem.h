#pragma once
#include "SystemBase.h"
#include "WorldRegistry.h"

namespace Twisted::Editor
{
	class SunSystem : public SystemBase
	{
		TCLASS_BODY()
	public:
		SunSystem(World* world) : SystemBase(world) {}

		void Update(float deltaTime) override;
	};
}


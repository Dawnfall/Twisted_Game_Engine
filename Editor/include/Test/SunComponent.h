#pragma once
#include "AComponent.h"
#include "Utils/YamlUtils.h"
#include "WorldRegistry.h"

namespace Twisted::Editor
{
	class SunComponent : public AComponent
	{
		TCLASS_BODY()
	public:
		SunComponent(Entity e) : AComponent(e), rotationSpeed(20.0f) {}

		TPROPERTY()
		float rotationSpeed;
	};
}


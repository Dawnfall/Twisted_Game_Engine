#pragma once
#include "AComponent.h"
#include "Utils/YamlUtils.h"
#include "WorldRegistry.h"

namespace Twisted::Editor
{
	class PlayerComponent : public AComponent
	{
		TCLASS_BODY()
	public:
		PlayerComponent(Entity e) : AComponent(e), moveSpeed(5.0f), lookSensitivity(0.003f) {}

		TPROPERTY()
		float moveSpeed;
		TPROPERTY()
		float lookSensitivity;
	};
}


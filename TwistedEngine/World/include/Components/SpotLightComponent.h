#pragma once

#include "AppCore.h"
#include "WorldRegistry.h"
#include "AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"
#include "Constants.h"

namespace Twisted
{
	struct TWISTED_API SpotLightComponent : public AComponent
	{
		TCLASS_BODY()
		SpotLightComponent(Entity e) : AComponent(e), color{}, intensity(1.0f), radius(1.0f), innerAngleDeg(30.0f), outerAngleDeg(60.0f) {}

		TPROPERTY()
		Vec4f color;
		TPROPERTY()
		float intensity;
		TPROPERTY()
		float radius;
		TPROPERTY()
		float innerAngleDeg;
		TPROPERTY()
		float outerAngleDeg;
	};
}


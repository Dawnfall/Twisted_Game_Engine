#pragma once

#include "AppCore.h"
#include "WorldRegistry.h"
#include "AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"
#include "Constants.h"

namespace Twisted
{
	struct TWISTED_API PointLightComponent : public AComponent
	{
		TCLASS_BODY()
		PointLightComponent(Entity e) : AComponent(e), color{}, intensity(1.0f), radius(1.0f) {}

		TPROPERTY()
		Vec4f color;
		TPROPERTY()
		float intensity;
		TPROPERTY()
		float radius;
	};
}


#pragma once

#include "AppCore.h"
#include "WorldRegistry.h"
#include "AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"
#include "Constants.h"

namespace Twisted
{
	struct TWISTED_API DirectionalLightComponent : public AComponent
	{
		TCLASS_BODY()
		DirectionalLightComponent(Entity e) : AComponent(e), color(0.0f, 0.0f, 0.0f, 0.0f), intensity(1.0f) {}

		TPROPERTY()
		Vec4f color;
		TPROPERTY()
		float intensity;
	};
}


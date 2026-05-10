#pragma once

#include "AppCore.h"
#include "WorldRegistry.h"
#include "AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"
#include "Constants.h"

namespace Twisted
{
	struct TWISTED_API DirectionalLightComponent :public AComponent
	{
		DirectionalLightComponent(Entity entity):AComponent(entity),color(0.0f,0.0f,0.0f,0.0f) {}

		Vec4f color;
		float intensity = 1.0f;
	};

	template<>
	inline YAML::Node YamlSerialize<DirectionalLightComponent>(const DirectionalLightComponent& light)
	{
		YAML::Node node;
		node[YAML_INTENSITY_FIELD] = light.intensity;
		node[YAML_COLOR_FIELD] = light.color;
		return node;
	}

	template<>
	inline void YamlDeserialize<DirectionalLightComponent>(DirectionalLightComponent& light, const YAML::Node& node)
	{
		if (node[YAML_INTENSITY_FIELD])
			light.intensity = node[YAML_INTENSITY_FIELD].as<float>();
		if (node[YAML_COLOR_FIELD])
			light.color = node[YAML_COLOR_FIELD].as<Vec4f>();
	}
}

REGISTER_COMPONENT(DirectionalLightComponent, "DirectionalLight");

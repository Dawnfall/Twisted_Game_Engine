#pragma once

#include "AppCore.h"
#include "WorldRegistry.h"
#include "AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"
#include "Constants.h"

namespace Twisted
{
	struct TWISTED_API SpotLightComponent :public AComponent
	{
		SpotLightComponent(Entity entity) :AComponent(entity) {}

		Vec4f color;
		float intensity = 1.0f;
		float radius = 1.0f;
		float innerAngleDeg = 30.0f;
		float outerAngleDeg = 60.0f;
	};

	template<>
	inline YAML::Node YamlSerialize<SpotLightComponent>(const SpotLightComponent& light)
	{
		YAML::Node node;
		node[YAML_INTENSITY_FIELD] = light.intensity;
		node[YAML_COLOR_FIELD] = light.color;
		node[YAML_RANGE_FIELD] = light.radius;
		node[YAML_INNER_FIELD] = light.innerAngleDeg;
		node[YAML_OUTER_FIELD] = light.outerAngleDeg;
		return node;
	}

	template<>
	inline void YamlDeserialize<SpotLightComponent>(SpotLightComponent& light, const YAML::Node& node)
	{
		if (node[YAML_INTENSITY_FIELD])
			light.intensity = node[YAML_INTENSITY_FIELD].as<float>();
		if (node[YAML_COLOR_FIELD])
			light.color = node[YAML_COLOR_FIELD].as<Vec4f>();
		if (node[YAML_RANGE_FIELD])
			light.radius = node[YAML_RANGE_FIELD].as<float>();
		if (node[YAML_INNER_FIELD])
			light.innerAngleDeg = node[YAML_INNER_FIELD].as<float>();
		if (node[YAML_OUTER_FIELD])
			light.outerAngleDeg = node[YAML_OUTER_FIELD].as<float>();
	}
}
REGISTER_COMPONENT(SpotLightComponent, "SpotLight");

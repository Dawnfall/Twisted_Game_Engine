#pragma once
#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/Gameing/AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"
#include "Twisted/Constants.h"

namespace Twisted
{
	struct SpotLightComponent :public AComponent
	{
		Vec4f color;
		float intensity;
		float radius;
		float innerAngleDeg;
		float outerAngleDeg;
	};

	template<>
	YAML::Node YamlSerialize<SpotLightComponent>(const SpotLightComponent& light)
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
	void YamlDeserialize<SpotLightComponent>(SpotLightComponent& light, const YAML::Node& node)
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

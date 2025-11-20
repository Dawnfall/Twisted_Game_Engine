#pragma once

#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/Gameing/AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	struct PointLightComponent :public AComponent
	{
		Vec4f color;
		float intensity;
		float radius;
	};

	template<>
	YAML::Node YamlSerialize<PointLightComponent>(const PointLightComponent& light)
	{
		YAML::Node node;
		node[YAML_INTENSITY_FIELD] = light.intensity;
		node[YAML_COLOR_FIELD] = light.color;
		node[YAML_RANGE_FIELD] = light.radius;
		return node;
	}

	template<>
	void YamlDeserialize<PointLightComponent>(PointLightComponent& light, const YAML::Node& node)
	{
		if (node[YAML_INTENSITY_FIELD])
			light.intensity = node[YAML_INTENSITY_FIELD].as<float>();
		if (node[YAML_COLOR_FIELD])
			light.color = node[YAML_COLOR_FIELD].as<Vec4f>();
		if (node[YAML_RANGE_FIELD])
			light.radius = node[YAML_RANGE_FIELD].as<float>();
	}
}

REGISTER_COMPONENT(PointLightComponent, "PointLight");

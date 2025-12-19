#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/Gameing/AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"
#include "Twisted/Constants.h"

namespace Twisted
{
	struct TWISTED_API PointLightComponent :public AComponent
	{
		PointLightComponent(Entity entity) :AComponent(entity) {}

		Vec4f color;
		float intensity=1.0f;
		float radius=1.0f;
	};

	template<>
	inline YAML::Node YamlSerialize<PointLightComponent>(const PointLightComponent& light)
	{
		YAML::Node node;
		node[YAML_INTENSITY_FIELD] = light.intensity;
		node[YAML_COLOR_FIELD] = light.color;
		node[YAML_RANGE_FIELD] = light.radius;
		return node;
	}

	template<>
	inline void YamlDeserialize<PointLightComponent>(PointLightComponent& light, const YAML::Node& node)
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

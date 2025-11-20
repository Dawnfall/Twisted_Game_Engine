#pragma once

#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/Gameing/AComponent.h"
#include "Utils/GlmUtils.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	struct DirectionalLightComponent :public AComponent
	{
		Vec4f color;
		float intensity;
	};

	template<>
	YAML::Node YamlSerialize<DirectionalLightComponent>(const DirectionalLightComponent& light)
	{
		YAML::Node node;
		node[YAML_INTENSITY_FIELD] = light.intensity;
		node[YAML_COLOR_FIELD] = light.color;
		return node;
	}

	template<>
	void YamlDeserialize<DirectionalLightComponent>(DirectionalLightComponent& light, const YAML::Node& node)
	{
		if (node[YAML_INTENSITY_FIELD])
			light.intensity = node[YAML_INTENSITY_FIELD].as<float>();
		if (node[YAML_COLOR_FIELD])
			light.color = node[YAML_COLOR_FIELD].as<Vec4f>();
	}
}

REGISTER_COMPONENT(DirectionalLightComponent, "DirectionalLight");

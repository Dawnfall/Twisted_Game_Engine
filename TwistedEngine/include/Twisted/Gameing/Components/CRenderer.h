#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	struct TWISTED_API RendererComponent :public AComponent
	{
		RendererComponent(Entity entity) :AComponent(entity) {}



		Material* material = nullptr;
		Mesh* mesh = nullptr;
	};

	template<>
	inline YAML::Node YamlSerialize<RendererComponent>(const RendererComponent& renderer)
	{
		YAML::Node node;

		//node["wind"] = YamlUtils::encodeTObject(renderer.WindOrder);
		//node["drawType"] = YamlUtils::encodeTObject(renderer.DrawType);

		node["mesh"] = YamlUtils::encodeTObject(renderer.mesh);
		node["mat"] = YamlUtils::encodeTObject(renderer.material);

		return node;
	}

	template<>
	inline void YamlDeserialize<RendererComponent>(RendererComponent& renderer, const YAML::Node& node)
	{
		//renderer.WindOrder = static_cast<Mesh*>(YamlUtils::decodeTObject(node["wind"]));
		//renderer.DrawType = static_cast<Mesh*>(YamlUtils::decodeTObject(node["drawType"]));

		renderer.mesh = static_cast<Mesh*>(YamlUtils::decodeTObject(node["mesh"]));
		renderer.material = static_cast<Material*>(YamlUtils::decodeTObject(node["mat"]));
	}

	
}

REGISTER_COMPONENT(RendererComponent, "CRenderer");


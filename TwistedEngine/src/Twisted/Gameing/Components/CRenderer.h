#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	class TWISTED_API CRenderer :public AComponent
	{
	public:
		CRenderer(Entity entity) :AComponent(entity) {}

		Material* material = nullptr;
		Mesh* mesh = nullptr;
	};

	template<>
	inline YAML::Node YamlSerialize<CRenderer>(const CRenderer& renderer)
	{
		YAML::Node node;

		node["mesh"] = YamlUtils::encodeTObject(renderer.mesh);
		node["mat"] = YamlUtils::encodeTObject(renderer.material);

		return node;
	}

	template<>
	inline void YamlDeserialize<CRenderer>(CRenderer& renderer, const YAML::Node& node)
	{
		renderer.mesh = static_cast<Mesh*>(YamlUtils::decodeTObject(node["mesh"]));
		renderer.material = static_cast<Material*>(YamlUtils::decodeTObject(node["mat"]));
	}

	
}

REGISTER_COMPONENT(CRenderer, "CRenderer");


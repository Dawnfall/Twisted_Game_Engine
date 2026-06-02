#pragma once

#include "AppCore.h"
#include "AComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "WorldRegistry.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	struct TWISTED_API RendererComponent : public AComponent
	{
		TCLASS_BODY()
		RendererComponent(Entity e) :AComponent(e) {}
		Material* material = nullptr;
		Mesh* mesh = nullptr;
	};

	template<>
	inline YAML::Node YamlSerialize<RendererComponent>([[maybe_unused]] const RendererComponent& renderer)
	{
		YAML::Node node;
		node["mesh"] = Twisted::encodeTObject(renderer.mesh);
		node["mat"]  = Twisted::encodeTObject(renderer.material);
		return node;
	}

	template<>
	inline void YamlDeserialize<RendererComponent>([[maybe_unused]] RendererComponent& renderer, [[maybe_unused]] const YAML::Node& node)
	{
		if (auto n = node["mesh"]; n.IsDefined() && !n.IsNull())
			renderer.mesh = static_cast<Mesh*>(Twisted::decodeTObject(n));
		if (auto n = node["mat"]; n.IsDefined() && !n.IsNull())
			renderer.material = static_cast<Material*>(Twisted::decodeTObject(n));
	}

	
}



#pragma once

#include "Dawn/Game/AComponent.h"
#include "Dawn/Rendering/Material.h"
#include "Dawn/Rendering/Mesh.h"

#include "EnTT/entt.hpp"

namespace Dawn
{
	struct CRenderer :public AComponent
	{
		CRenderer(entt::entity id) :AComponent(id) {}

		std::shared_ptr<Material> Material;
		std::shared_ptr<Mesh> Mesh;
	};
}
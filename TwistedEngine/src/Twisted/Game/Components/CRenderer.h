#pragma once

#include "Twisted/Game/AComponent.h"
#include "Twisted/Rendering/Material.h"
#include "Twisted/Rendering/Mesh.h"

#include "EnTT/entt.hpp"

namespace Twisted
{
	struct CRenderer :public AComponent
	{
		CRenderer(entt::entity id) :AComponent(id) {}

		std::shared_ptr<Material> Material;
		std::shared_ptr<Mesh> Mesh;
	};
}
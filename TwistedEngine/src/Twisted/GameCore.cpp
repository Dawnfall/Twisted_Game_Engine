#include "pch.h"
#include "GameCore.h"
#include "Game/Components/CTransform.h"

namespace Twisted
{
	std::vector<CTransform*> GameCore::GetRootTransforms()
	{
		std::vector<CTransform*> result;
			auto view = Ecs.GetComponents<CTransform>();
			for (auto entity : view)
			{
				CTransform& transform = view.get<CTransform>(entity);
				if (transform.GetParentID() == entt::null)
				{
					result.push_back(&transform);
				}
			}
		return result;
	}
}
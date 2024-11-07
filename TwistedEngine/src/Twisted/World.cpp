#include "twistedpch.h"
#include "World.h"

#include "Game/Components/CTransform.h"

namespace Twisted
{
	void World::UpdateFrame(AppBase* app)
	{
		m_time.UpdateNewFrame();

		for (auto& system : m_systems)
			system->Update(app);
	}

	CTransform& World::CreateNewEntity()
	{
		auto newEntt = m_ecs.CreateEntity();
		return m_ecs.AddComponent<CTransform>(newEntt);
	}

	std::vector<CTransform*> World::GetRootTransforms()
	{
		std::vector<CTransform*> result;
		auto view = m_ecs.GetComponents<CTransform>();
		for (auto entity : view)
		{
			CTransform& transform = view.get<CTransform>(entity);
			if (transform.GetParentID() == entt::null)
			{
				result.emplace_back(&transform);
			}
		}
		return result;
	}
}
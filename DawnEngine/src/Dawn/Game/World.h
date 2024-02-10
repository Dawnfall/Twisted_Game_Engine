#pragma once
#include "EnTT/entt.hpp"

#include "Dawn/Rendering/Mesh.h"
namespace Dawn
{
	class World
	{
	public:
		entt::entity CreateEntity();
		
		entt::registry& GetRegistry();

		template <typename... ComponentTypes>
		void AddComponent(entt::entity id)
		{
			//static_assert(std::is_base_of<AComponent, ComponentTypes && ...>::value);
			(m_registry.emplace<ComponentTypes>(id), ...);
		}

		template <typename... ComponentTypes>
		void RemoveComponents(entt::entity id)
		{
			//static_assert(std::is_base_of<AComponent, ComponentTypes && ...>::value);
			(m_registry.remove<ComponentTypes>(id), ...);
		}

		template <typename T>
		T GetComponent(entt::entity id)
		{
			//static_assert(std::is_base_of<AComponent, T>::value);
			return m_registry.get<T>(id);
		}

		void Test()
		{
			//TODO: get all components with renderer
			auto meshes = m_registry.view<Mesh>();
		}

	private:
		entt::registry m_registry;
	};

}
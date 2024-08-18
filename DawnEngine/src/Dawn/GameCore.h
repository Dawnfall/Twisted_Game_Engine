#pragma once

#include "Dawn/Game/AComponent.h"
#include "Dawn/SystemsCore.h"
#include "Utils/Utils.h"

#include <EnTT/entt.hpp>
#include <type_traits>
#include <vector>

namespace Dawn
{
	struct AComponent;

	class GameCore
	{
	public:
		void DestroyAll()
		{
			m_registry.clear();
		}

		entt::entity CreateEntity();
		std::vector<entt::entity> CreateEntities(int amount);
		void DestroyEntity(entt::entity id);
		void DestroyEntities(const std::vector<entt::entity>& entities);

		//TODO:... continue here
		template<typename T>
		T& AddComponent(entt::entity id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "Component must be of type AComponent");
			return m_registry.emplace<T>(id, id);
		}

		template <typename... ComponentTypes>
		void AddComponents(entt::entity id)
		{
			//TODO: static assert variadic pack	
			(m_registry.emplace<ComponentTypes>(id, id), ...);
		}

		template<typename Iterator, typename... ComponentTypes>
		void AddComponents(Iterator begin, Iterator end)//(const std::vector <entt::entity>& entities)
		{
			for (Iterator it = begin; it != end; it++)
				(m_registry.insert<ComponentTypes>(*it, *it), ...);
		}

		template<typename T>
		void RemoveComponent(entt::entity id)
		{
			m_registry.remove<T>(id); //or erase if we know
		}

		template <typename... ComponentTypes>
		void RemoveComponents(entt::entity id)
		{
			(m_registry.remove<ComponentTypes>(id), ...);
		}

		template<typename T>
		void DestroyAllOfType()
		{
			m_registry.clear<T>();
		}

		template <typename T>
		const T& GetComponent(entt::entity id)const
		{
			static_assert(std::is_base_of<AComponent, T>::value);
			return m_registry.get<T>(id);
		}


		SystemsCore Systems;

		entt::registry& GetRegistry() { return m_registry; }
	private:
		entt::registry m_registry;
	};
}
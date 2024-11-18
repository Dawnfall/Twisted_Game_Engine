#pragma once

#include "twistedpch.h"
#include "AppCore.h"
#include "Twisted/Managers/TimeManager.h"
#include "Twisted/Game/SystemBase.h"


namespace Twisted
{
	class AComponent;
	class CTransform;
	class TWISTED_API World //TODO: error handling
	{
	public:
		EntityID CreateNewEntity(EntityID id);

		template<typename... ComponentTypes>
		EntityID CreateEntity(EntityID id)
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");

			EntityID entityID = CreateNewEntity(id);
			(m_registry.emplace<ComponentTypes>(entityID, entityID, this), ...);

			return entityID;
		}

		template<typename FirstComponent, typename... OtherComponents>
		FirstComponent* AddComponents(EntityID entityID)
		{
			static_assert(std::is_base_of<AComponent, FirstComponent>::value, "FirstComponent must derive from AComponent");
			static_assert((std::is_base_of<AComponent, OtherComponents>::value && ...), "All types must derive from AComponent");
			static_assert(!std::is_same<CTransform, FirstComponent>::value, "Component types must not be CTransform");
			static_assert((!std::is_same<CTransform, OtherComponents>::value && ...), "Component types must not be CTransform");

			FirstComponent& firstComponent = m_registry.emplace<FirstComponent>(entityID, entityID, this);
			(m_registry.emplace<OtherComponents>(entityID, entityID, this), ...);
			return &firstComponent;
		}

		template <typename T>
		T* GetComponent(EntityID entityID)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(entityID);
		}

		template <typename T>
		const T* GetComponent(EntityID entityID)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(entityID);
		}

		template<typename... ComponentTypes>
		auto GetComponents()
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			return m_registry.view<ComponentTypes...>();
		}

		template<typename... ComponentTypes>
		auto GetComponents() const
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			return m_registry.view<ComponentTypes...>();
		}

		template<typename... ComponentTypes>
		void DestroyOfType()
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
			(m_registry.clear<ComponentTypes>(), ...);
		}

		void Clear()
		{
			m_registry.clear();
		}

		void DestroyEntity(EntityID entityID)
		{
			m_registry.destroy(entityID);
		}

		std::vector<EntityID> GetAllEntityIDs() const
		{
			std::vector<EntityID> entityIDs;
			entityIDs.reserve(m_registry.size()); // Pre-allocate memory for performance
			m_registry.each([&entityIDs](auto entity) {
				entityIDs.push_back(entity);
				});
			return entityIDs;
		}

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			m_systems.emplace_back(std::make_unique<T>());
		}

		void UpdateFrame(AppBase* app, TimeManager& time);

		template<typename... ComponentTypes>
		void RemoveComponents(EntityID entityID)
		{
			// Static asserts to ensure that each type in ComponentTypes is a component and not CTransform
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");

			// Remove each component type from the entity
			(m_registry.remove<ComponentTypes>(entityID), ...);
		}

	private:
		entt::registry m_registry;
		std::vector<std::unique_ptr<SystemBase>> m_systems;

		friend class WorldSerializer;
	};


}

#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/SystemBase.h"
#include "Entity.h"
#include "Twisted/Application/Application.h"
#include "Twisted/Data/Serializer.h"

#include "Twisted/UUIDObject.h"
#include "Components/CTransform.h"
#include "Components/CName.h"

namespace Twisted
{
	class TWISTED_API World :public UUIDObject
	{
	public:
		World(Application* app);

		Application& GetApplication() { return *m_app; }
		const std::vector<EntityID>& GetRootEntities()const { return m_rootEntities; }

		void UpdateFrame();

		void Clear()
		{
			m_registry.clear();
			m_systems.clear();
			m_rootEntities.clear();
		}

		template<typename... ComponentTypes>
		EntityID CreateEntity()
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
			static_assert((!std::is_same<CName, ComponentTypes>::value && ...), "Component types must not be CName");

			EntityID entityID = m_registry.create();
			m_registry.emplace<CTransform>(entityID, entityID, this);
			m_registry.emplace<CName>(entityID, entityID, this);
			(m_registry.emplace<ComponentTypes>(entityID, entityID, this), ...);

			return entityID;
		}

		void DestroyEntity(EntityID entityID)
		{
			m_registry.destroy(entityID);
		}

		std::vector<EntityID> GetAllEntityIDs() const
		{
			std::vector<EntityID> entityIDs;
			entityIDs.reserve(m_registry.storage<entt::entity>()->size()); // Pre-allocate memory for performance
			
			auto view = m_registry.view<CName>(); // we can keep track ourselves
			for (auto entity : view)
			{
				entityIDs.push_back(entity);
			}
			return entityIDs;
		}

		template<typename FirstComponent, typename... OtherComponents>
		FirstComponent* AddComponents(EntityID entityID)
		{
			static_assert(std::is_base_of<AComponent, FirstComponent>::value, "FirstComponent must derive from AComponent");
			static_assert((std::is_base_of<AComponent, OtherComponents>::value && ...), "All types must derive from AComponent");
			static_assert(!std::is_same<CTransform, FirstComponent>::value, "Component types must not be CTransform");
			static_assert((!std::is_same<CTransform, OtherComponents>::value && ...), "Component types must not be CTransform");
			static_assert(!std::is_same<CName, FirstComponent>::value, "Component types must not be CName");
			static_assert((!std::is_same<CName, OtherComponents>::value && ...), "Component types must not be CName");

			FirstComponent& firstComponent = m_registry.emplace<FirstComponent>(entityID, entityID, this);
			(m_registry.emplace<OtherComponents>(entityID, entityID, this), ...);
			return &firstComponent;
		}

		template<typename... ComponentTypes>
		void RemoveComponents(EntityID entityID)
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");

			(m_registry.remove<ComponentTypes>(entityID), ...);
		}

		template <typename T>
		T& GetComponent(EntityID entityID)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.get<T>(entityID);
		}

		template <typename T>
		const T& GetComponent(EntityID entityID)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.get<T>(entityID);
		}

		template <typename T>
		T* TryGetComponent(EntityID entityID)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(entityID);
		}

		template<typename T>
		bool HasComponent(EntityID entityID)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.any_of<T>(entityID);
		}

		template<typename... ComponentTypes>
		bool HasComponents(EntityID entityID)
		{
			static_assert(std::is_base_of<AComponent, ComponentTypes>::value, "T must derive from AComponent");
			return m_registry.all_of<ComponentTypes...>(entityID);
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

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			m_systems.emplace_back(std::make_unique<T>(this));
		}

		//Serialization
		void Serialize(BinSerializer& buffer)const;
		void Deserialize(BinSerializer& buffer);

		template<typename T>
		void serializeComponents(BinSerializer& buffer)const
		{
			auto componentsView = GetComponents<T>();
			buffer.Write<size_t>(componentsView.size());
			for (auto entId : componentsView)
			{
				const T component = componentsView.get<0>(entId);
				buffer.Write<EntityID>(component.GetEntityID());
				component.Serialize(buffer);
			}
			buffer.Write<std::vector<EntityID>>(m_rootEntities);
		}

		template<typename T>
		void deSerializeComponents(BinSerializer& buffer)
		{
			size_t compCount = buffer.Read<size_t>();
			for (size_t i = 0; i < compCount; i++)
			{
				EntityID entityID = buffer.Read<EntityID>();
				T& component = m_registry.emplace<T>(entityID, entityID, this);
				component.Deserialize(buffer);
			}
			m_rootEntities = buffer.Read<std::vector<EntityID>>();
		}

	private:

		Application* m_app;
		entt::registry m_registry;
		std::vector<std::unique_ptr<SystemBase>> m_systems;
		std::vector<EntityID> m_rootEntities;

		friend class CTransform;
	};
}

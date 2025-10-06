#pragma once

#include "AppCore.h"
#include "Twisted/Application/Application.h"
#include "Twisted/RegisterLayer/TObject.h"
#include "Twisted/Gameing/SystemBase.h"
#include "Twisted/Gameing/ComponentRegistry.h"
#include <unordered_map>

#include <EnTT/entt.hpp>

#include "Components/CTransform.h"
#include "Components/CName.h"

namespace Twisted
{
	class TWISTED_API World :public TObject
	{
	public:
		World(Application* app);

		auto GetAllEntities() const { return m_registry.storage<entt::entity>(); }

		std::vector<EntityID> GetAllEntitiesAsVector()const
		{
			std::vector<EntityID> allEntities;
			auto allEnts = GetAllEntities();
			//std::copy(allEnts->begin(), allEnts->end(), allEntities);
			return allEntities;
		}

		const std::vector<EntityID>& GetRootEntities()const { return m_rootEntities; }
		std::vector<EntityID>& GetRootEntities() { return m_rootEntities; }

		void UpdateFrame();

		void Clear();

		Application* GetApplication() { return m_app; }

		template<typename... ComponentTypes>
		EntityID CreateEntity()
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
			static_assert((!std::is_same<CName, ComponentTypes>::value && ...), "Component types must not be CName");

			EntityID newEntity = m_registry.create();

			AddComponent<CTransform>(newEntity);
			AddComponent<CName>(newEntity);
			(AddComponent<ComponentTypes>(newEntity), ...);

			return newEntity;
		}

		template<typename T, typename ... Args>
		T& AddComponent(EntityID entity, Args&&... arguments)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			T& newComponent = m_registry.emplace<T>(entity, entity, this, std::forward<Args>(arguments)...);
			newComponent.Init();
			return newComponent;
		}

		template<typename... Components>
		void AddComponents(EntityID entity)
		{
			static_assert((std::is_base_of<AComponent, Components>::value && ...), "All types must derive from AComponent");

			(AddComponent<Components>(entity), ...);
		}

		template<typename... ComponentTypes>
		void RemoveComponents(EntityID entity)
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");

			(m_registry.remove<ComponentTypes>(entity), ...);
		}

		template <typename T>
		T& GetComponent(EntityID entity)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.get<T>(entity);
		}

		template <typename T>
		const T& GetComponent(EntityID entity)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.get<T>(entity);
		}

		template <typename T>
		const T* TryGetComponent(EntityID entity)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(entity);
		}

		template <typename T>
		T* TryGetComponent(EntityID entity)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.any_of<T>(entity);
		}

		template<typename... ComponentTypes>
		bool HasComponents(EntityID entity)
		{
			static_assert(std::is_base_of<AComponent, ComponentTypes>::value, "T must derive from AComponent");
			return m_registry.all_of<ComponentTypes...>(entity);
		}

		void DestroyEntity(EntityID entity);

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

		bool IsValid(EntityID entity)const { return m_registry.valid(entity); }

		BinSerializer Serialize(AssetsLayer* assetsLayer)const;
		void Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer);
		//void PostSerialize(BinSerializer& buffer)const;
		//void PostDeserialize(BinSerializer& buffer);

	private:
		entt::registry m_registry;

		std::vector<std::unique_ptr<SystemBase>> m_systems;
		std::vector<EntityID> m_rootEntities;
		
		Application* m_app;

		friend class WorldImporter;
		friend class ComponentRegistry;
		friend class CTransform;
	};

}


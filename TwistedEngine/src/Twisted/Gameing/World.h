#pragma once

#include "AppCore.h"
#include "Twisted/Application/Application.h"
#include "Twisted/TObject.h"
#include "Twisted/Gameing/SystemBase.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Serialization/BinSerializer.h"
#include "Entity.h"

#include <EnTT/entt.hpp>

#include <unordered_map>

namespace Twisted
{
	class TWISTED_API World :public TObject
	{
	public:
		World(const std::string& name);

		auto GetAllEntities() const { return m_registry.storage<entt::entity>(); }

		template<typename T>
		const T* GetSystem()const
		{
			for (auto& system : m_systems)
			{
				T* castSystem = dynamic_cast<T*>(system.get());
				if (castSystem)
					return castSystem;
			}
			return nullptr;
		}

		template<typename T>
		T* GetSystem()
		{
			for (auto& system : m_systems)
			{
				T* castSystem = dynamic_cast<T*>(system.get());
				if (castSystem)
					return castSystem;
			}
			return nullptr;
		}

		entt::registry& GetRegistry() { return m_registry; }
		const entt::registry& GetRegistry() const { return m_registry; }

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			if (HasSystem<T>())
				return;
			m_systems.emplace_back(std::make_unique<T>(this));
		}

		void Clear();

		//Application* GetApplication() { return m_app; }

		const std::vector<EntityID>& GetRootEntities()const { return m_rootEntities; }
		std::vector<EntityID>& GetRootEntities() { return m_rootEntities; }

		void UpdateFrame();

		template<typename T>
		bool HasSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			for (auto& system : m_systems)
				if (dynamic_cast<T*>(system.get()))
					return true;
			return false;
		}





		std::vector<URef<SystemBase>>& GetAllSystems() { return m_systems; }
		const std::vector<URef<SystemBase>>& GetAllSystems()const { return m_systems; }

		template<typename... ComponentTypes>
		inline Entity CreateNew(ComponentTypes&&... args)
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
			static_assert((!std::is_same<CName, ComponentTypes>::value && ...), "Component types must not be CName");

			Entity newEntity{ m_registry.create(),this };

			AddComponent<CTransform>(newEntity.GetID());
			AddComponent<CName>(newEntity.GetID());
			(AddComponent<ComponentTypes>(newEntity.GetID()), ...);

			return newEntity;
		}

		void DestroyEntity(EntityID id)
		{
			if (m_registry.valid(id))
				m_registry.destroy(id);
		}

		template<typename T, typename ... Args>
		T& AddComponent(EntityID id, Args&&... args)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			T& newComponent = m_registry.emplace<T>(id, Entity{ id,this }, std::forward<Args>(args)...);
			newComponent.Init();
			return newComponent;
		}

		template<typename... ComponentTypes>
		void AddComponents(EntityID id)
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			(AddComponent<ComponentTypes>(id), ...);
		}

		template<typename... ComponentTypes>
		void RemoveComponents(EntityID id)
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");

			(m_registry.remove<ComponentTypes>(id), ...);
		}

		template<typename T>
		bool HasComponent(EntityID id)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.any_of<T>(id);
		}

		template<typename... ComponentTypes>
		bool HasComponents(EntityID id)const
		{
			static_assert((std::is_base_of_v<AComponent, ComponentTypes> && ...),
				"All types must derive from AComponent");
			return m_registry.all_of<ComponentTypes...>(id);
		}

		template <typename T>
		T& GetComponent(EntityID id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.get<T>(id);
		}

		template <typename T>
		const T& GetComponent(EntityID id)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.get<T>(id);
		}

		template <typename T>
		T* TryGetComponent(EntityID id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(id);
		}

		template <typename T>
		const T* TryGetComponent(EntityID id)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return m_registry.try_get<T>(id);
		}

		template<typename... ComponentTypes>
		auto GetView() {
			static_assert((std::is_base_of_v<AComponent, ComponentTypes> && ...),
				"All types must derive from AComponent");
			return m_registry.view<ComponentTypes...>();
		}

		template<typename... ComponentTypes>
		auto GetView() const {
			static_assert((std::is_base_of_v<AComponent, ComponentTypes> && ...),
				"All types must derive from AComponent");
			return m_registry.view<ComponentTypes...>();
		}

		template<typename Owned, typename... GetComponents>
		auto GetGroup() {
			static_assert(std::is_base_of_v<AComponent, Owned>, "Owned type must derive from AComponent");
			static_assert((std::is_base_of_v<AComponent, GetComponents> && ...),
				"All types must derive from AComponent");
			return m_registry.group<Owned, GetComponents...>();
		}

		template<typename Owned, typename... GetComponents>
		auto GetGroup() const {
			static_assert(std::is_base_of_v<AComponent, Owned>, "Owned type must derive from AComponent");
			static_assert((std::is_base_of_v<AComponent, GetComponents> && ...),
				"All types must derive from AComponent");
			return m_registry.group<Owned, GetComponents...>();
		}

		template<typename T>
		T* FindFirstOfType()
		{
			static_assert(std::is_base_of_v<AComponent, T>, "T must derive from AComponent");

			auto view = m_registry.view<T>();
			for (auto entity : view)
				return &view.get<T>(entity); // Return pointer to first one

			return nullptr; // None found
		}

		template<typename T>
		void RemoveComponent(EntityID id)
		{
			static_assert(std::is_base_of_v<AComponent, T>, "T must derive from AComponent");
			static_assert(!std::is_same_v<CTransform, T>, "Cannot remove CTransform component");

			if (m_registry.any_of<T>(id))
				m_registry.remove<T>(id);
		}

		template<typename T>
		void RemoveAllComponents()
		{
			static_assert(std::is_base_of_v<AComponent, T>, "T must derive from AComponent");
			static_assert(!std::is_same_v<CTransform, T>, "Cannot remove all CTransform components");

			m_registry.clear<T>();
		}

		YAML::Node YamlSerialize()const;

		void YamlDeserialize(const YAML::Node& node);

	private:
		entt::registry m_registry;

		std::vector<std::unique_ptr<SystemBase>> m_systems;
		std::vector<EntityID> m_rootEntities;

		friend class WorldImporter;
		friend class CTransform;
		friend class WorldSerializer;
	};


	inline void writeToBuffer(const EntityID& entityID, BinSerializer& buffer, void* data)
	{
		buffer.Write(static_cast<uint32_t>(entityID), nullptr);
	}

	template<>
	inline EntityID readFromBuffer<EntityID>(BinSerializer& buffer, void* data)
	{
		auto id = buffer.Read<uint32_t>(nullptr);
		return static_cast<EntityID>(id);
	}

}


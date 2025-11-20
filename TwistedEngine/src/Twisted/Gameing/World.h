#pragma once

#include "AppCore.h"
#include "Debug/Logger.h"

#include "Twisted/Gameing/SystemBase.h"
#include "Twisted/TObject.h"
#include "Twisted/Gameing/Entity.h"

#include "Utils/YamlUtils.h"

#include <entt/entt.hpp>
#include <memory>
#include <vector>
#include <string>

namespace Twisted
{
	class TWISTED_API World :public TObject
	{
	public:
		World(const std::string& name);

		entt::registry& GetRegistry() { return m_registry; }
		const entt::registry& GetRegistry() const { return m_registry; }
		auto GetAllEntities() const { return m_registry.storage<entt::entity>(); }

		//********
		// Managers
		//********

		template<typename T>
		T& ForceGetManager()
		{
			if (T* obj = m_registry.ctx().find<T>(); obj)
				return *obj;
			return m_registry.ctx().emplace<T>(this);
		}
		template<typename T>
		T* GetManager()
		{
			return m_registry.ctx().find<T>();
		}
		template<typename T>
		const T* GetManager()const
		{
			return m_registry.ctx().find<T>();
		}

		//********
		// Systems
		//********

		const std::vector<URef<SystemBase>>& GetAllSystems() const { return m_systems; }
		std::vector<URef<SystemBase>>& GetAllSystems() { return m_systems; }

		template<typename T>
		bool HasSystem()const
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			return m_registry.ctx().find<T>() != nullptr;
		}

		template<typename T>
		T* GetSystem() {
			for (auto& sys : m_systems) {
				if (auto* ptr = dynamic_cast<T*>(sys.get()))
					return ptr;
			}
			return nullptr;
		}

		template<typename T>
		const T* GetSystem() const {
			for (auto& sys : m_systems) {
				if (auto* ptr = dynamic_cast<T*>(sys.get()))
					return ptr;
			}
			return nullptr;
		}

		template<typename T>
		T& AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			if (T* sys = GetSystem<T>(); sys != nullptr)
				return *sys;
			URef<T> newSys = std::make_unique<T>(this);
			T* sysPtr = newSys.get();
			m_systems.emplace_back(std::move(newSys));
			return *sysPtr;
		}

		//********
		// Components
		//********

		void Clear();

		//Application* GetApplication() { return m_app; }

		void UpdateFrame();

		Entity CreateNewEntity();

		template<typename... ComponentTypes>
		inline Entity CreateNewEntityWithComponents()
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
			static_assert((!std::is_same<CName, ComponentTypes>::value && ...), "Component types must not be CName");

			Entity newEntity = CreateNewEntity();
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
			static_assert(std::is_base_of_v<AComponent, T>, "T must derive from AComponent");
			if (T* comp = m_registry.try_get<T>(id); comp)
			{
				TWISTED_INFO("entity already contains component");
				return *comp;
			}

			T& newComponent = m_registry.emplace<T>(id, Entity{ id,this }, std::forward<Args>(args)...);
			return newComponent;
		}

		template<typename... ComponentTypes>
		void AddComponents(EntityID id)
		{
			(AddComponent<ComponentTypes>(id), ...);
		}

		template<typename T>
		void RemoveComponent(EntityID id)
		{
			static_assert(std::is_base_of_v<AComponent, T>, "All types must derive from AComponent");
			static_assert(!std::is_same_v<CTransform, T>, "Component types must not be CTransform");

			if (m_registry.valid(id))
			{
				if (T* comp = m_registry.try_get<T>(id); comp)
				{
					m_registry.remove<T>(id);
				}
			}
		}

		template<typename... ComponentTypes>
		void RemoveComponents(EntityID id)
		{
			(RemoveComponent<ComponentTypes>(id), ...);
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
		void RemoveAllComponents()
		{
			static_assert(std::is_base_of_v<AComponent, T>, "T must derive from AComponent");
			static_assert(!std::is_same_v<CTransform, T>, "Cannot remove all CTransform components");

			m_registry.clear<T>();
		}

	private:
		entt::registry m_registry;
		std::vector<URef<SystemBase>> m_systems;

		friend class WorldImporter;
		friend class CTransform;

		friend YAML::Node YamlSerialize<World>(const World& world);
		friend void YamlDeserialize<World>(World& world, const YAML::Node& node);
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

	template<>
	YAML::Node YamlSerialize<World>(const World& world);

	template<>
	void YamlDeserialize<World>(World& world, const YAML::Node& data);
}




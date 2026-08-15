#pragma once

#include "AppCore.h"
#include "Debug/Logger.h"

#include "SystemBase.h"
#include "ManagerBase.h"
#include "AComponent.h"
#include "Entity.h"

namespace Twisted { class NameComponent; }
#include "Application/TObject.h"

#include "Utils/YamlUtils.h"

#include <entt/entt.hpp>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>

template<typename T>
concept SystemType = std::derived_from<T, Twisted::SystemBase>;
template<typename T>
concept ManagerType = std::derived_from<T, Twisted::ManagerBase>;
template<typename T>
concept ComponentType = std::derived_from<T, Twisted::AComponent>;
template<typename T>
concept RemovableComponentType = std::derived_from<T, Twisted::AComponent> && (!std::same_as<T, Twisted::TransformComponent>) && (!std::same_as<T, Twisted::NameComponent>);

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

		template<ManagerType T>
		T& ForceGetManager()
		{
			if (T* obj = m_registry.ctx().find<T>(); obj)
				return *obj;
			return m_registry.ctx().emplace<T>(this);
		}

		template<ManagerType T>
		T* GetManager()
		{
			return m_registry.ctx().find<T>();
		}

		template<ManagerType T>
		const T* GetManager()const
		{
			return m_registry.ctx().find<T>();
		}

		//********
		// Systems
		//********

		const std::vector<URef<SystemBase>>& GetAllSystems() const { return m_systems; }

		std::vector<URef<SystemBase>>& GetAllSystems() { return m_systems; }

		template<SystemType T>
		bool HasSystem()const
		{
			return m_registry.ctx().find<T>() != nullptr;
		}

		template<SystemType T>
		T* GetSystem() {
			for (auto& sys : m_systems) {
				if (auto* ptr = dynamic_cast<T*>(sys.get()))
					return ptr;
			}
			return nullptr;
		}

		template<SystemType T>
		const T* GetSystem() const {
			for (auto& sys : m_systems) {
				if (auto* ptr = dynamic_cast<T*>(sys.get()))
					return ptr;
			}
			return nullptr;
		}

		template<SystemType T>
		T& AddSystem()
		{
			if (T* sys = GetSystem<T>(); sys != nullptr)
				return *sys;
			URef<T> newSys = std::make_unique<T>(this);
			T* sysPtr = newSys.get();
			m_systems.emplace_back(std::move(newSys));
			return *sysPtr;
		}

		template<SystemType T>
		void RemoveSystem()
		{
			auto it = std::find_if(m_systems.begin(), m_systems.end(), [](const URef<SystemBase>& sys) {
				return dynamic_cast<T*>(sys.get()) != nullptr;
			});
			if (it != m_systems.end())
				m_systems.erase(it);
		}

		//********
		// Components
		//********

		void Clear();

		//Application* GetApplication() { return m_app; }

		void UpdateFrame(float deltaTime);

		Entity CreateNewEntity();

		template<RemovableComponentType... ComponentTypes>
		inline Entity CreateNewEntityWithComponents()
		{
			Entity newEntity = CreateNewEntity();
			(AddComponent<ComponentTypes>(newEntity.GetID()), ...);

			return newEntity;
		}

		void DestroyEntity(EntityID id)
		{
			if (m_registry.valid(id))
				m_registry.destroy(id);
		}

		template<ComponentType T, typename ... Args>
		T& AddComponent(EntityID id, Args&&... args)
		{
			if (T* comp = m_registry.try_get<T>(id); comp)
			{
				TWISTED_INFO("entity already contains component");
				return *comp;
			}

			T& newComponent = m_registry.emplace<T>(id, Entity{ id,this }, std::forward<Args>(args)...);
			return newComponent;
		}

		template<ComponentType... ComponentTypes>
		void AddComponents(EntityID id)
		{
			(AddComponent<ComponentTypes>(id), ...);
		}

		template<RemovableComponentType T>
		void RemoveComponent(EntityID id)
		{
			if (m_registry.valid(id))
			{
				if (T* comp = m_registry.try_get<T>(id); comp)
				{
					m_registry.remove<T>(id);
				}
			}
		}

		template<RemovableComponentType... ComponentTypes>
		void RemoveComponents(EntityID id)
		{
			(RemoveComponent<ComponentTypes>(id), ...);
		}

		template<ComponentType T>
		bool HasComponent(EntityID id)const
		{
			return m_registry.any_of<T>(id);
		}

		template<ComponentType... ComponentTypes>
		bool HasComponents(EntityID id)const
		{
			return m_registry.all_of<ComponentTypes...>(id);
		}

		template <ComponentType T>
		T& GetComponent(EntityID id)
		{
			return m_registry.get<T>(id);
		}

		template<ComponentType... Ts>
		std::tuple<Ts...> GetComponents(EntityID id)
		{
			return m_registry.get<Ts...>(id);
		}

		template <ComponentType T>
		const T& GetComponent(EntityID id)const
		{
			return m_registry.get<T>(id);
		}

		template <ComponentType T>
		T* TryGetComponent(EntityID id)
		{
			return m_registry.try_get<T>(id);
		}

		template <ComponentType T>
		const T* TryGetComponent(EntityID id)const
		{
			return m_registry.try_get<T>(id);
		}

		template<ComponentType... ComponentTypes>
		auto GetView() {
			return m_registry.view<ComponentTypes...>();
		}

		template<ComponentType... ComponentTypes>
		auto GetView() const {
			return m_registry.view<ComponentTypes...>();
		}

		template<ComponentType Owned, ComponentType... GetComponents>
		auto GetGroup() {
			return m_registry.group<>(entt::get<Owned, GetComponents...>);
		}

		template<ComponentType Owned, ComponentType... GetComponents>
		auto GetGroup() const {
			return m_registry.group<>(entt::get<Owned, GetComponents...>);
		}

		template<ComponentType Owned, ComponentType... GetComponents>
		auto GetOwningGroup() {
			return m_registry.group<Owned, GetComponents...>();
		}

		template<ComponentType Owned, ComponentType... GetComponents>
		auto GetOwningGroup() const {
			return m_registry.group<Owned, GetComponents...>();
		}

		template<ComponentType T>
		T* FindFirstOfType()
		{
			auto view = m_registry.view<T>();
			if (!view.empty())
				return &view.template get<T>(*view.begin());

			return nullptr;
		}

		template<ComponentType T>
		const T* FindFirstOfType()const
		{
			auto view = m_registry.view<T>();
			if (!view.empty())
				return &view.template get<T>(*view.begin());

			return nullptr;
		}

		template<RemovableComponentType T>
		void RemoveAllComponents()
		{
			m_registry.clear<T>();
		}

	private:
		entt::registry m_registry;
		std::vector<URef<SystemBase>> m_systems;

		friend class WorldImporter;
		friend class TransformComponent;
	};

	inline void writeToBuffer(const EntityID& entityID, BinSerializer& buffer, [[maybe_unused]] void* data)
	{
		buffer.Write(static_cast<uint32_t>(entityID), nullptr);
	}

	template<>
	inline EntityID readFromBuffer<EntityID>(BinSerializer& buffer, [[maybe_unused]] void* data)
	{
		auto id = buffer.Read<uint32_t>(nullptr);
		return static_cast<EntityID>(id);
	}

	template<>
	YAML::Node YamlSerialize<World>(const World& world);

	template<>
	void YamlDeserialize<World>(World& world, const YAML::Node& data);
}




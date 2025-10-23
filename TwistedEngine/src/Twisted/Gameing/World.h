#pragma once

#include "AppCore.h"
#include "Twisted/Application/Application.h"
#include "Twisted/TObject.h"
#include "Twisted/Gameing/SystemBase.h"
#include <unordered_map>
#include <EnTT/entt.hpp>

#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Serialization/BinSerializer.h"

#include "Entity.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"

namespace Twisted
{
	//template <typename F, typename... Ts>
	//void for_each_type(type_list<Ts...>, F&& f) {
	//	(f.template operator() < Ts > (), ...);
	//}

	class TWISTED_API World :public TObject
	{
	public:
		World() = default;
		//World(Application* app);

		auto GetAllEntities() const { return m_registry.storage<entt::entity>(); }

		entt::registry& GetRegistry() { return m_registry; }
		const entt::registry& GetRegistry() const { return m_registry; }

		void Clear();

		//Application* GetApplication() { return m_app; }

		const std::vector<EntityID>& GetRootEntities()const { return m_rootEntities; }
		std::vector<EntityID>& GetRootEntities() { return m_rootEntities; }

		void UpdateFrame();

		template<typename T>
		void AddSystem()
		{
			static_assert(std::is_base_of <SystemBase, T>::value, "T must derive from SystemBase");
			m_systems.emplace_back(std::make_unique<T>(this));
		}


		template<typename... ComponentTypes>
		inline Entity CreateNew(ComponentTypes&&... args)
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
			static_assert((!std::is_same<CName, ComponentTypes>::value && ...), "Component types must not be CName");

			Entity newEntity{ GetRegistry().create(),this };

			AddComponent<CTransform>(newEntity.GetID());
			AddComponent<CName>(newEntity.GetID());
			(AddComponent<ComponentTypes>(newEntity.GetID()), ...);

			return newEntity;
		}

		void DestroyEntity(EntityID id)
		{
			if (GetRegistry().valid(id))
				GetRegistry().destroy(id);
		}

		template<typename T, typename ... Args>
		T& AddComponent(EntityID id,Args&&... args)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			T& newComponent = GetRegistry().emplace<T>(id,Entity{ id,this }, std::forward<Args>(args)...);
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

			(GetRegistry().remove<ComponentTypes>(id), ...);
		}

		template<typename T>
		bool HasComponent(EntityID id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return GetRegistry().any_of<T>(id);
		}

		template<typename... ComponentTypes>
		bool HasComponents(EntityID id)
		{
			static_assert(std::is_base_of<AComponent, ComponentTypes>::value, "T must derive from AComponent");
			return GetRegistry().all_of<ComponentTypes...>(id);
		}

		template <typename T>
		T& GetComponent(EntityID id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return GetRegistry().get<T>(id);
		}

		template <typename T>
		const T& GetComponent(EntityID id)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return GetRegistry().get<T>(id);
		}

		template <typename T>
		T* TryGetComponent(EntityID id)
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return GetRegistry().try_get<T>(id);
		}

		template <typename T>
		const T* TryGetComponent(EntityID id)const
		{
			static_assert(std::is_base_of<AComponent, T>::value, "T must derive from AComponent");
			return GetRegistry().try_get<T>(id);
		}

		template<typename... ComponentTypes>
		inline auto GetComponents()
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			return GetRegistry().view<ComponentTypes...>();
		}

		template<typename... ComponentTypes>
		inline void DestroyOfType()
		{
			static_assert((std::is_base_of<AComponent, ComponentTypes>::value && ...), "All types must derive from AComponent");
			static_assert((!std::is_same<CTransform, ComponentTypes>::value && ...), "Component types must not be CTransform");
			(GetRegistry().clear<ComponentTypes>(), ...);
		}

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


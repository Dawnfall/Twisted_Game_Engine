#pragma once

#include "AppCore.h"
#include "World.h"

namespace Twisted
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityID id, World* world) :m_id(id), m_world(world) {}

		bool IsValid()const { return m_world && m_world->IsValid(m_id); }

		const EntityID GetID()const { return m_id; }
		const World* GetWorld()const { return m_world; }
		World* GetWorld() { return m_world; }

		explicit operator bool() const noexcept { return IsValid(); }
		bool operator==(const Entity other) const { return other.m_world == m_world && other.m_id == m_id; }
		bool operator!=(const Entity other) const { return !((*this) == other); }


		template<typename T, typename ... Args>
		T& AddComponent(Args&&... args)
		{
			return m_world->AddComponent<T>(m_id, std::forward<Args>(args)...);
		}

		template<typename... Components>
		void AddComponents()
		{
			(m_world->AddComponent<Components>(m_id), ...);
		}

		template<typename... Components>
		void RemoveComponents()
		{
			(m_world->RemoveComponents<Components>(m_id), ...);
		}

		template <typename T>
		T& GetComponent()
		{
			return m_world->GetComponent<T>(m_id);
		}

		template <typename T>
		const T& GetComponent()const
		{
			return m_world->GetComponent<T>(m_id);
		}

		template <typename T>
		T* TryGetComponent()
		{
			return m_world->TryGetComponent<T>(m_id);
		}

		template <typename T>
		const T* TryGetComponent()const
		{
			return m_world->TryGetComponent<T>(m_id);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_world->HasComponent<T>(m_id);
		}

		template<typename... ComponentTypes>
		bool HasComponents()
		{
			return m_world->HasComponents<ComponentTypes>(m_id);
		}

		template<typename... Args>
		inline static Entity CreateNew(World* world, Args&&... args)
		{
			EntityID newID = world->CreateEntity<Args...>(std::forward<Args>(args)...);
			return Entity(newID, world);
		}

		void DestroyEntity()
		{
			m_world->DestroyEntity(m_id);
		}

	private:
		EntityID m_id = NullEntity;
		World* m_world = nullptr;
	};

	//inline bool operator==(const Entity& a, const Entity& b) noexcept
	//{
	//	return a.GetID() == b.GetID() && a.GetWorld() == b.GetWorld();
	//}
}

namespace std {
	template<>
	struct hash<Twisted::Entity>
	{
		size_t operator()(const Twisted::Entity& e) const noexcept
		{
			size_t h1 = std::hash<Twisted::EntityID>{}(e.GetID());
			size_t h2 = std::hash<const Twisted::World*>{}(e.GetWorld());
			return h1 ^ (h2 << 1); // combine hashes
		}
	};
}
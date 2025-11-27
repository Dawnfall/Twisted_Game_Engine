#pragma once

#include "AppCore.h"
#include "Serialization/BinSerializer.h"
#include <yaml-cpp/yaml.h>
#include <entt/entt.hpp>

namespace Twisted
{
	using EntityID = entt::entity;
	constexpr entt::entity NullEntity = entt::null;

	class World;
	class TransformComponent;
	class NameComponent;
	class AComponent;

	class TWISTED_API Entity
	{
	public:
		Entity(EntityID id, World* world) :m_id(id), m_world(world) {}

		const EntityID& GetID()const { return m_id; }
		const World* GetWorld()const { return m_world; }
		World* GetWorld() { return m_world; }

		bool IsValid()const;

		explicit operator bool() const noexcept { return IsValid(); }
		bool operator==(const Entity other) const { return other.m_world == m_world && other.m_id == m_id; }
		bool operator!=(const Entity other) const { return !((*this) == other); }

		static const Entity& Invalid()
		{
			static Entity invalidEntity(NullEntity, nullptr);
			return invalidEntity;
		}

	private:

		EntityID m_id = NullEntity;
		World* m_world = nullptr;
	};


	inline void writeToBuffer(const Entity& entity, BinSerializer& buffer, void* data)
	{
		buffer.Write<EntityID>(entity.GetID(), nullptr);
	}

	template<>
	inline Entity readFromBuffer<Entity>(BinSerializer& buffer, void* data)
	{
		World* world = static_cast<World*>(data);
		return Entity{ buffer.Read<EntityID>(nullptr),world };
	}
}

namespace std
{
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

namespace YAML
{
	template<>
	struct convert<Twisted::EntityID>
	{
		static Node encode(const Twisted::EntityID& rhs)
		{
			YAML::Node node;
			node = static_cast<uint32_t>(rhs);
			return node;
		}

		static bool decode(const YAML::Node& node, Twisted::EntityID& rhs)
		{
			if (!node || !node.IsScalar())
				throw std::runtime_error("YAML::convert<entt::entity>::decode: Invalid or missing YAML node");

			rhs = static_cast<Twisted::EntityID>(node.as<uint32_t>());
			return true;
		}
	};
}


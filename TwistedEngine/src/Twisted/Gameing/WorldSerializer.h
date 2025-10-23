#pragma once
#include "AComponent.h"
#include "World.h"
#include "Entity.h"
#include "Serialization/BinSerializer.h"
#include "Debug/Logger.h"

#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <functional>

namespace Twisted
{
	template<typename T>
	constexpr const char* GetComponentName() = delete;

	using DeserializeFn = std::function<void(World&, BinSerializer&, EntityID)>;
	using SerializeFn = std::function<void(const World&, BinSerializer&)>;

	class WorldSerializer
	{
	public:
		static WorldSerializer& GetInstance()
		{
			static WorldSerializer instance;
			return instance;
		}

		template<typename T>
		void RegisterComponent()
		{
			static_assert(std::is_base_of_v<AComponent, T>, "T must derive from AComponent");
			std::string compName = GetComponentName<T>();

			m_serializers[compName] = [this](const World& world, BinSerializer& buffer) {
				this->SerializeComponents<T>(world, buffer);
				};

			m_deserializers[compName] = [this](World& world, BinSerializer& buffer, EntityID id) {
				this->DeserializeComponent<T>(world, buffer, id);
				};
		}

		BinSerializer Serialize(const World& world)
		{
			Twisted::BinSerializer buffer;
			buffer.Write<std::vector<EntityID>>(world.m_rootEntities, nullptr);

			for (auto& pair : m_serializers)
			{
				buffer.Write<std::string>(pair.first, nullptr);
				pair.second(world, buffer);
			}
			return buffer;
		}

		World* Deserialize(BinSerializer& buffer) const
		{

			World* world = TObject::Create<World>();
			world->m_rootEntities = buffer.Read<std::vector<EntityID>>(nullptr);

			while (true) //TODO:... need to break somehow
			{
				std::string compName = buffer.Read<std::string>(nullptr);
				size_t compCount = buffer.Read<size_t>(nullptr);

				auto it = m_deserializers.find(compName);

				for (size_t i = 0; i < compCount; ++i)
				{
					EntityID entId = buffer.Read<EntityID>(nullptr);
					size_t compSize = buffer.Read<size_t>(nullptr);

					if (it != m_deserializers.end())
					{
						DeserializeFn deserFn = it->second;
						deserFn(*world, buffer, entId);
					}
					else
					{
						TWISTED_WARN("Unknown component when deserializing: " + compName);
						buffer.SkipReadIndex(compSize);
					}
				}
			}
			return world;
		}

	private:



		template<typename T>
		void SerializeComponents(const World& world, BinSerializer& buffer)
		{
			auto view = world.GetRegistry().view<T>();
			buffer.Write<size_t>(view.size(), nullptr);

			for (auto entity : view)
			{
				buffer.Write<EntityID>(entity, nullptr);

				size_t positionToPatch = buffer.GetSize();
				buffer.Write<size_t>(0, nullptr);
				view.get<T>(entity).Serialize(buffer);

				size_t componentSize = buffer.GetSize() - positionToPatch - sizeof(size_t);
				buffer.PatchBytes<size_t>(componentSize, positionToPatch);
			}
		}

		template<typename T>
		void DeserializeComponent(World& world, BinSerializer& buffer, EntityID entID)
		{
			if (!world.GetRegistry().valid(entID))
				entID = world.GetRegistry().create(entID);

			//T& newComponent = world.GetRegistry().emplace<T>(entID, entID, &world);
			//newComponent.Deserialize(buffer);
		}

	private:

		std::unordered_map<std::string, SerializeFn> m_serializers;
		std::unordered_map<std::string, DeserializeFn> m_deserializers;

	public:
		WorldSerializer(const WorldSerializer& other) = delete;
		WorldSerializer(WorldSerializer&& other) = delete;
		WorldSerializer& operator=(const WorldSerializer& other) = delete;
		WorldSerializer& operator=(WorldSerializer&& other) = delete;

	private:

		WorldSerializer() = default;
	};
}
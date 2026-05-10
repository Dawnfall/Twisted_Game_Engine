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

#include "SystemBase.h"
#include "yaml-cpp/yaml.h"
#include "Utils/YamlUtils.h"

template<typename T>
constexpr const char* GetTypeName() = delete;

template<typename T, typename = void>
struct has_on_create_component : std::false_type {};

template<typename T, typename = void>
struct has_on_destroy_component : std::false_type {};

template<typename T>
struct has_on_create_component<T, std::void_t<decltype(OnCreateComponent<T>(std::declval<T&>()))>> : std::true_type {};

template<typename T>
struct has_on_destroy_component<T, std::void_t<decltype(OnDestroyComponent<T>(std::declval<T&>()))>> : std::true_type {};

namespace Twisted
{
	template<typename T>
	struct OnEnttEvent {
		static void onCreate(entt::registry& reg, entt::entity entity)
		{
			if constexpr (has_on_create_component<T>::value) {
				T& comp = reg.get<T>(entity);
				OnCreateComponent<T>(comp);
			}
		}
		static void onDestroy(entt::registry& reg, entt::entity entity)
		{
			if constexpr (has_on_destroy_component<T>::value) {
				T& comp = reg.get<T>(entity);
				OnDestroyComponent<T>(comp);
			}
		}
	};

	class TWISTED_API WorldRegistry
	{
	public:

		struct RegManagerEntry
		{
			std::function<YAML::Node(const World&)> YamlSerMethod;
			std::function<void(World&, const YAML::Node&)> YamlDeserMethod;
		};

		struct RegSystemEntry
		{
			std::function<YAML::Node(const World&)> YamlSerMethod;
			std::function<void(World&, const YAML::Node&)> YamlDeserMethod;
			std::function<bool(World&)> HasSystemMethod;
			std::function<void(World&)> AddSystemMethod;
		};

		struct RegComponentEntry
		{
			std::function<YAML::Node(const World&)> YamlSerMethod;
			std::function<void(World&, const YAML::Node&)> YamlDeserMethod;
			std::function<AComponent* (Entity)> GetComponentMethod;
			std::function<bool(Entity)> HasComponentMethod;
			std::function<void(Entity)> AddComponentMethod;
			std::function<void(Entity)> RemoveComponentMethod;
			std::function<void(World& world)> InitAndDestroyRegisterMethod;
		};

		template<typename T>
		void RegisterManager()
		{
			//static_assert(std::is_base_of_v<ManagerBase, T>, "T must derive from ManagerBase");
			std::string managerName = GetTypeName<T>();

			RegManagerEntry& entry = m_registeredManagers[managerName];

			entry.YamlSerMethod = [](const World& world)->YAML::Node {
				const T* manager = world.GetManager<T>();
				if (manager)
					return YamlSerialize<T>(*manager);
				return YAML::Node{};
				};
			entry.YamlDeserMethod = [](World& world, const YAML::Node& node) {
				T& manager = world.ForceGetManager<T>();
				return YamlDeserialize<T>(manager, node);
				};
		}
		template<typename T>
		void RegisterSystem()
		{
			static_assert(std::is_base_of_v<SystemBase, T>, "T must derive from SystemBase");
			std::string systemName = GetTypeName<T>();

			RegSystemEntry& entry = m_registeredSystem[systemName];

			entry.AddSystemMethod = [](World& world) {
				world.AddSystem<T>();
				};
			entry.HasSystemMethod = [](World& world) {
				return world.HasSystem<T>();
				};
			entry.YamlSerMethod = [](const World& world)->YAML::Node {
				if (world.HasSystem<T>())
				{
					const T* system = world.GetSystem<T>();
					if (system)
						return YamlSerialize<T>(*system);
				}
				return YAML::Node{};
				};
			entry.YamlDeserMethod = [](World& world, const YAML::Node& node) {
				T& system = world.AddSystem<T>();
				YamlDeserialize<T>(system, node);
				};
		}
		template<typename T>
		void RegisterComponent()
		{
			static_assert(std::is_base_of_v<AComponent, T>, "T must derive from AComponent");
			std::string compName = GetTypeName<T>();

			RegComponentEntry& entry = m_registeredComponents[compName];

			entry.InitAndDestroyRegisterMethod = [](World& world) {
				world.GetRegistry().on_construct<T>().template connect<&OnEnttEvent<T>::onCreate>();
				world.GetRegistry().on_destroy<T>().template connect<&OnEnttEvent<T>::onDestroy>();
				};
			entry.GetComponentMethod = [](Entity entity)->AComponent* {
				return entity.GetWorld()->TryGetComponent<T>(entity.GetID());
				};
			entry.AddComponentMethod = [](Entity entity) {
				entity.GetWorld()->AddComponent<T>(entity.GetID());
				};
			entry.HasComponentMethod = [](Entity entity)->bool {
				return entity.GetWorld()->HasComponent<T>(entity.GetID());
				};
			if constexpr (RemovableComponentType<T>)
			{
				entry.RemoveComponentMethod = [](Entity entity) {
					entity.GetWorld()->RemoveComponent<T>(entity.GetID());
					};
			}
			entry.YamlSerMethod = [](const World& world)->YAML::Node {
				YAML::Node allCompNode;
				auto view = world.GetRegistry().view<T>();
				for (auto entity : view)
				{
					allCompNode[entity] = YamlSerialize<T>(view.template get<T>(entity));
				}
				return allCompNode;
				};
			entry.YamlDeserMethod = [](World& world, const YAML::Node allCompNode) {
				for (auto it = allCompNode.begin(); it != allCompNode.end(); ++it)
				{
					EntityID entID = (*it).first.as<EntityID>(NullEntity);
					const YAML::Node& entNode = (*it).second;

					if (!world.GetRegistry().valid(entID))
						entID = world.GetRegistry().create(entID);

					T& newComponent = world.GetRegistry().emplace<T>(entID, Entity{ entID, &world });
					YamlDeserialize<T>(newComponent, entNode);
				}
				};
		}



		const auto& GetSystemEntries()const { return m_registeredSystem; }
		const auto& GetComponentEntries()const { return m_registeredComponents; }
		const auto& GetManagerEntries()const { return m_registeredManagers; }
		const RegSystemEntry* GetSystemEntry(const std::string& typeName)const
		{
			auto it = m_registeredSystem.find(typeName);
			if (it != m_registeredSystem.end())
				return &it->second;
			return nullptr;
		}
		const RegComponentEntry* GetComponentEntry(const std::string& typeName)const
		{
			auto it = m_registeredComponents.find(typeName);
			if (it != m_registeredComponents.end())
				return &it->second;
			return nullptr;
		}

	public:
		std::unordered_map<std::string, RegManagerEntry> m_registeredManagers;
		std::unordered_map<std::string, RegSystemEntry> m_registeredSystem;
		std::unordered_map<std::string, RegComponentEntry> m_registeredComponents;

	private:
		WorldRegistry() = default;

	public:
		static WorldRegistry& GetInstance();

		WorldRegistry(const WorldRegistry& other) = delete;
		WorldRegistry(WorldRegistry&& other) = delete;
		WorldRegistry& operator=(const WorldRegistry& other) = delete;
		WorldRegistry& operator=(WorldRegistry&& other) = delete;
	};
}

#define REGISTER_COMPONENT(C,Name)										\
template<> inline const char* GetTypeName<Twisted::C>() { return Name; }					\
namespace Registry														\
{																		\
    struct C##AutoRegister												\
    {                                                                   \
        C##AutoRegister()												\
        {                                                               \
          Twisted::WorldRegistry::GetInstance().RegisterComponent<Twisted::C>();	\
        }																			\
    } inline s_##C##AutoRegister;															\
}

#define REGISTER_SYSTEM(S,Name)										\
template<> inline const char* GetTypeName<Twisted::S>() { return Name; }					\
namespace Registry														\
{																		\
    struct S##AutoRegister												\
    {                                                                   \
        S##AutoRegister()												\
        {                                                               \
          Twisted::WorldRegistry::GetInstance().RegisterSystem<Twisted::S>();	\
        }																			\
    } inline s_##S##AutoRegister;															\
}

#define REGISTER_MANAGER(M,Name)										\
template<> inline const char* GetTypeName<Twisted::M>() { return Name; }					\
namespace Registry														\
{																		\
    struct M##AutoRegister												\
    {                                                                   \
        M##AutoRegister()												\
        {                                                               \
          Twisted::WorldRegistry::GetInstance().RegisterManager<Twisted::M>();	\
        }																			\
    } inline s_##M##AutoRegister;															\
}


//using DeserializeFn = std::function<void(World&, BinSerializer&, EntityID)>;
//using SerializeFn = std::function<void(const World&, BinSerializer&)>;
//
//std::unordered_map<std::string, SerializeFn> m_serializers;
//std::unordered_map<std::string, DeserializeFn> m_deserializers;
//
// 		template<typename T>
//void DeserializeComponent(World& world, BinSerializer& buffer, EntityID entID)
//{
//	if (!world.GetRegistry().valid(entID))
//		entID = world.GetRegistry().create(entID);
//
//	//T& newComponent = world.GetRegistry().emplace<T>(entID, entID, &world);
//	//newComponent.Deserialize(buffer);
//}
// 
//template<typename T>
//void SerializeComponents(const World& world, BinSerializer& buffer)
//{
//	auto view = world.GetRegistry().view<T>();
//	buffer.Write<size_t>(view.size(), nullptr);
//
//	for (auto entity : view)
//	{
//		buffer.Write<EntityID>(entity, nullptr);
//
//		size_t positionToPatch = buffer.GetSize();
//		buffer.Write<size_t>(0, nullptr);
//		view.get<T>(entity).Serialize(buffer);
//
//		size_t componentSize = buffer.GetSize() - positionToPatch - sizeof(size_t);
//		buffer.PatchBytes<size_t>(componentSize, positionToPatch);
//	}
//}
//
//
//m_serializers[compName] = [this](const World& world, BinSerializer& buffer) {
//	this->SerializeComponents<T>(world, buffer);
//	};
//
//m_deserializers[compName] = [this](World& world, BinSerializer& buffer, EntityID id) {
//	this->DeserializeComponent<T>(world, buffer, id);
//	};


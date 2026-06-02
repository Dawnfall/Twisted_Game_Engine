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
#include "Reflection/AutoSerialize.h"

// Specializations are defined in each type's .gen.cpp — resolved at link time.
template<typename T>
const char* GetTypeName();

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
			std::function<const std::vector<PropertyInfo>*()> GetPropertiesMethod;
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
			entry.GetPropertiesMethod = []() -> const std::vector<PropertyInfo>* {
				const auto& props = GetTypeProperties<T>();
				return props.empty() ? nullptr : &props;
				};
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

// Place inside a class/struct body to mark it for code generation by twistedgen.py.
// The generator finds the enclosing class declaration automatically — no arguments needed.
// Dissolves to nothing at compile time; all output goes to the .gen.cpp file.
#define TCLASS_BODY()

// Marks a field for code generation by twistedgen.py.
// Dissolves to nothing at compile time; the .gen.cpp file provides all registrations.
// Place immediately before the field declaration:
//   TPROPERTY()
//   float myField;
// Also accepts old 3-arg form TPROPERTY(Type, Name, "Display") — the field declaration
// is then written explicitly after the macro.
#define TPROPERTY(...)


// Indirection layer forces __COUNTER__ to expand before ## token-pasting.
#define TCLASS_IMPL_(FullType, DisplayName, Kind, Counter)                         \
    template<> inline const char* GetTypeName<FullType>() { return DisplayName; } \
    namespace Registry                                                             \
    {                                                                              \
        struct TAutoReg_##Counter                                                  \
        {                                                                          \
            TAutoReg_##Counter()                                                   \
            {                                                                      \
                Twisted::WorldRegistry::GetInstance().Register##Kind<FullType>(); \
            }                                                                      \
        };                                                                         \
        static TAutoReg_##Counter s_TAutoReg_##Counter;                           \
    }
#define TCLASS_IMPL(FullType, DisplayName, Kind, Counter) \
    TCLASS_IMPL_(FullType, DisplayName, Kind, Counter)

// Generates YamlSerialize/YamlDeserialize specializations that delegate to AutoYamlSerialize.
// Used by TCOMPONENT for types whose fields are fully described by TPROPERTY.
#define TCOMPONENT_YAML_IMPL(FullType)                                                           \
    namespace Twisted                                                                            \
    {                                                                                           \
        template<> inline YAML::Node YamlSerialize<FullType>(const FullType& obj)              \
        { return ::Twisted::AutoYamlSerialize(obj); }                                          \
        template<> inline void YamlDeserialize<FullType>(FullType& obj, const YAML::Node& node)\
        { ::Twisted::AutoYamlDeserialize(obj, node); }                                        \
    }

// Registration + auto YAML serialization (use when all serialized fields are marked TPROPERTY).
#define TCOMPONENT(FullType, DisplayName)        \
    TCLASS_IMPL(FullType, DisplayName, Component, __COUNTER__) \
    TCOMPONENT_YAML_IMPL(FullType)

// Registration only — for types that provide their own YamlSerialize/YamlDeserialize.
#define TCOMPONENT_MANUAL(FullType, DisplayName) \
    TCLASS_IMPL(FullType, DisplayName, Component, __COUNTER__)

#define TSYSTEM(FullType, DisplayName)    TCLASS_IMPL(FullType, DisplayName, System,    __COUNTER__)
#define TMANAGER(FullType, DisplayName)   TCLASS_IMPL(FullType, DisplayName, Manager,   __COUNTER__)


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


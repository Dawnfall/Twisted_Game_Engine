#include "World.h"
#include "SystemBase.h"

#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"

//#include "ComponentRegistry.h"

#include "Twisted/AssetsLayer/AssetsService.h"
#include "AppCore.h"
#include <vector>
#include "WorldRegistry.h"
#include <yaml-cpp/node/node.h>



namespace Twisted
{
	World::World(const std::string& name) :TObject(name)
	{
		for (const auto& entry : WorldRegistry::GetInstance().GetComponentEntries())
		{
			entry.second.InitAndDestroyRegisterMethod(*this);
		}
	}

	void World::Clear()
	{
		m_registry.clear();
	}
	void World::UpdateFrame(float deltaTime)
	{
		for (auto& system : m_systems)
			system->Update(deltaTime);
	}

	Entity World::CreateNewEntity()
	{
		Entity newEntity{ m_registry.create(),this };
		AddComponent<TransformComponent>(newEntity.GetID());
		AddComponent<NameComponent>(newEntity.GetID());

		return newEntity;
	}

	YAML::Node YamlSerialize<World>(const World& world)
	{
		YAML::Node node;

		auto managersNode = node[MANAGERS_SER_KEY];
		for (auto& [managerName, entry] : WorldRegistry::GetInstance().GetManagerEntries())
		{
			managersNode[managerName] = entry.YamlSerMethod(world);
		}
		auto systemsNode = node[SYSTEMS_SER_KEY];
		for (auto& [systemName, entry] : WorldRegistry::GetInstance().GetSystemEntries())
		{
			systemsNode[systemName] = entry.YamlSerMethod(world);
		}
		auto allCompNode = node[COMPONENTS_SER_KEY];
		for (auto& [compName, entry] : WorldRegistry::GetInstance().GetComponentEntries())
		{
			allCompNode[compName] = entry.YamlSerMethod(world);
		}
		return node;
	}

	void YamlDeserialize<World>(World& world, const YAML::Node& node)
	{
		const auto& allCompNode = node[COMPONENTS_SER_KEY];
		for (auto& [compName, entry] : WorldRegistry::GetInstance().GetComponentEntries())
		{
			if (!allCompNode[compName])
				continue;

			const auto& thisCompNode = allCompNode[compName];
			entry.YamlDeserMethod(world, thisCompNode);
		}

		const auto& allSystemNode = node[SYSTEMS_SER_KEY];
		for (auto& [systemName, entry] : WorldRegistry::GetInstance().GetSystemEntries())
		{
			if (!allSystemNode[systemName])
				continue;

			const auto& thisSystemNode = allSystemNode[systemName];
			entry.YamlDeserMethod(world, thisSystemNode);
		}

		const auto& allManagersNode = node[MANAGERS_SER_KEY];
		for (auto& [managerName, entry] : WorldRegistry::GetInstance().GetManagerEntries())
		{
			if (!allManagersNode[managerName])
				continue;

			const auto& thisManagerNode = allManagersNode[managerName];
			entry.YamlDeserMethod(world, thisManagerNode);
		}
	}
}

//
//World::World(const std::string& name, BinSerializer& buffer) :TObject(name)
//{
//	m_rootEntities = buffer.Read<std::vector<EntityID>>(nullptr);
//	while (true) //TODO:... need to break somehow
//	{
//		std::string compName = buffer.Read<std::string>(nullptr);
//		size_t compCount = buffer.Read<size_t>(nullptr);
//
//		auto it = WorldRegistry::GetInstance().m_yamlComponentDeserializers.find(compName);
//
//		for (size_t i = 0; i < compCount; ++i)
//		{
//			EntityID entId = buffer.Read<EntityID>(nullptr);
//			size_t compSize = buffer.Read<size_t>(nullptr);
//
//			if (it != WorldRegistry::GetInstance().m_yamlComponentDeserializers.end())
//			{
//				DeserializeFn deserFn = it->second;
//				deserFn(*this, buffer, entId);
//			}
//			else
//			{
//				TWISTED_WARN("Unknown component when deserializing: " + compName);
//				buffer.SkipReadIndex(compSize);
//			}
//		}
//	}
//}

//BinSerializer World::Serialize()const
//{
//	Twisted::BinSerializer buffer;
//	buffer.Write<std::vector<EntityID>>(m_rootEntities, nullptr);
//
//	for (auto& pair : WorldRegistry::GetInstance().m_serializers)
//	{
//		buffer.Write<std::string>(pair.first, nullptr);
//		pair.second(*this, buffer);
//	}
//	return buffer;
//}


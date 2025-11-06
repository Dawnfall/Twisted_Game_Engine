#include "World.h"
#include "SystemBase.h"

#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"
#include "Twisted/Gameing/Systems/RenderSystem.h"

//#include "ComponentRegistry.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "AppCore.h"
#include <vector>
#include "WorldRegistry.h"

namespace Twisted
{
	World::World(const std::string& name) :TObject(name) {}

	void World::Clear()
	{
		m_registry.clear();
		m_systems.clear();
	}
	void World::UpdateFrame()
	{
		for (auto& system : m_systems)
			system->Update();
	}



	YAML::Node World::YamlSerialize()const
	{
		YAML::Node node;

		YAML::Node systemsNode=node["systems"];
		for (auto& system : m_systems)
		{
			auto name = system->GetTypeName();
			const auto entry = WorldRegistry::GetInstance().GetSystemEntry(name);
			if (entry)
				entry->YamlSerMethod(*system);
		}
		YAML::Node allCompNode = node["components"];
		for (auto& [compName, entry] : WorldRegistry::GetInstance().GetComponentEntries())
		{
			allCompNode[compName] = entry.YamlSerMethod(*this);
		}
		node["roots"] = m_rootEntities;
		return node;
	}

	void World::YamlDeserialize(const YAML::Node& data)
	{
		if (const YAML::Node systemsNode = data["systems"]; systemsNode && systemsNode.IsMap())
		{
			for (const auto& sysNode : systemsNode)
			{
				std::string sysName = sysNode.first.as<std::string>();
				const auto entry = WorldRegistry::GetInstance().GetSystemEntry(sysName);
				if (entry)
					m_systems.emplace_back(entry->YamlDeserMethod(*this, sysNode.second));
			}
		}
		if (const YAML::Node& allCompNode = data["components"]; allCompNode && allCompNode.IsMap())
		{
			for (auto it = allCompNode.begin(); it != allCompNode.end(); ++it)
			{
				const std::string compName = it->first.as<std::string>("");
				const YAML::Node& compNode = it->second;
				WorldRegistry::GetInstance().GetComponentEntry(compName)->YamlDeserMethod(*this, compNode);
			}
		}
		m_rootEntities = data["roots"].as<std::vector<EntityID>>();

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
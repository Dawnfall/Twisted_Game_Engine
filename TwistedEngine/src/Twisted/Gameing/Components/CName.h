#pragma once

#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/Gameing/AComponent.h"
#include "Serialization/BinSerializer.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	class TWISTED_API CName :public AComponent
	{
	public:
		CName(Entity entity) :AComponent(entity),
			Name(std::to_string(static_cast<int>(entity.GetID())))
		{}

		std::string Name;
	};

	template<>
	inline YAML::Node YamlSerialize<CName>(const CName& name)
	{
		YAML::Node node;

		node["name"] = name.Name;

		return node;
	}
	template<>
	inline void YamlDeserialize<CName>(CName& name, const YAML::Node& node)
	{
		name.Name = node["name"].as<std::string>();
	}
	template<typename T>
	inline void BinSerialize(const CName& name, BinSerializer& buffer)
	{
		buffer.Write<std::string>(name.Name, nullptr);
	}
	template<typename T>
	inline void BinDeserialize(CName& name, BinSerializer& buffer)
	{
		name.Name = buffer.Read<std::string>(nullptr);
	}
}

REGISTER_COMPONENT(CName, "CName");


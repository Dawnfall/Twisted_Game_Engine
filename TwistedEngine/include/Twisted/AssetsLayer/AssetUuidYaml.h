#pragma once
#include "AssetUuid.h"
#include <yaml-cpp/yaml.h>

template<>
struct YAML::convert<Twisted::AssetUuid>
{
	static YAML::Node encode(const Twisted::AssetUuid id)
	{
		return YAML::Node(id.ToString());
	}

	static bool decode(const YAML::Node& node, Twisted::AssetUuid& uuid)
	{
		if (!node.IsScalar())
			return false;
		uuid = Twisted::AssetUuid::FromString(node.Scalar());
		return true;
	}
};

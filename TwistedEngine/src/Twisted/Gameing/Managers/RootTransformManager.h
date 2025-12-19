#pragma once

#include "AppCore.h"
#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/Gameing/ManagerBase.h"
#include "Twisted/Gameing/Entity.h"

#include "Utils/YamlUtils.h"
#include <vector>

namespace Twisted
{
	inline const std::string ROOTS_KEY = "roots";

	class TWISTED_API RootTransformManager :public ManagerBase
	{
	public:
		RootTransformManager(World* world) :ManagerBase(world) {}

		std::vector<EntityID> RootEntities;
	private:
	};

	template<>
	inline YAML::Node YamlSerialize<RootTransformManager>(const RootTransformManager& manager)
	{
		YAML::Node node;
		node[ROOTS_KEY] = manager.RootEntities;
		return node;
	}

	template<>
	inline void YamlDeserialize<RootTransformManager>(RootTransformManager& rootManager, const YAML::Node& node)
	{

		if (const auto& roots = node[ROOTS_KEY]; roots && roots.IsSequence())
			rootManager.RootEntities = node[ROOTS_KEY].as<std::vector<EntityID>>();
	}
}

REGISTER_MANAGER(RootTransformManager,"Transform Manager")

#pragma once

#include "AppCore.h"
#include "WorldRegistry.h"
#include "ManagerBase.h"
#include "Entity.h"

#include "Utils/YamlUtils.h"
#include <vector>

namespace Twisted
{
	inline constexpr const char* ROOTS_KEY = "roots";

	class TWISTED_API RootTransformManager : public ManagerBase
	{
		TCLASS_BODY()
	public:
		RootTransformManager(World* w) :ManagerBase(w) {}

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



#pragma once

#include "WorldRegistry.h"
#include "AComponent.h"
#include "Utils/YamlUtils.h"

namespace Twisted
{
	class TWISTED_API NameComponent : public AComponent
	{
		TCLASS_BODY()
	public:
		NameComponent(Entity e) : AComponent(e),
			Name(std::to_string(static_cast<int>(e.GetID())))
		{}

		TPROPERTY()
		std::string Name;
	};
}


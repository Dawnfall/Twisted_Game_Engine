#include "Twisted/Gameing/WorldRegistry.h"

namespace Twisted
{
	WorldRegistry& WorldRegistry::GetInstance()
	{
		static WorldRegistry instance;
		return instance;
	}
}


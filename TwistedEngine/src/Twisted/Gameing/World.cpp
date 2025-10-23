#include "World.h"
#include "SystemBase.h"

#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Components/CName.h"
#include "Twisted/Gameing/Systems/RenderSystem.h"

//#include "ComponentRegistry.h"

#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "AppCore.h"
#include <vector>

namespace Twisted
{
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
}
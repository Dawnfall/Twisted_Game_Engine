#include "twistedpch.h"
#include "StandaloneLayer.h"

#include "Twisted/Game/World.h"
#include "Twisted/Game/Systems/RenderSystem.h"

namespace Twisted
{
	void StandaloneLayer::OnInit()
	{
		m_world = std::make_shared<World>(m_app);
		m_world->AddSystem<RenderSystem>(); 
	}
	void StandaloneLayer::OnFrame()
	{
		if (m_world)
			m_world->UpdateFrame();
	}
}
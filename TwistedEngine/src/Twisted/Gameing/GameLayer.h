#pragma once
#include "Twisted/Application/Layer.h"
#include "Twisted/Gameing/World.h"
#include <vector>
namespace Twisted
{
	class GameLayer :public Layer
	{
	public:
		GameLayer(Application* app):Layer(app){}

		World* CreateNewWorld()
		{
			m_worlds.emplace_back(std::make_unique<World>(m_app));
			return m_worlds.back().get();
		}

		bool DestroyWorld(World* world)
		{
			for(int i=0;i<m_worlds.size();i++)
				if (m_worlds.at(i).get() == world)
				{
					m_worlds.erase(m_worlds.begin() + i);
					return true;
				}
			return false;
		}

	private:
		std::vector<std::unique_ptr<World>> m_worlds;
	};
}
#pragma once

#include <memory>
#include "Dawn/Game/World.h"

namespace Dawn
{
	class GameCore
	{
	public:
		void Init();
		void Update();
	private:
		std::unique_ptr<World> m_world;
	};

}
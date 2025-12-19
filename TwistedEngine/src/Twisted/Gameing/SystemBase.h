#pragma once

#include "AppCore.h"

#include <yaml-cpp/yaml.h>

namespace Twisted
{
	class World;
	class TWISTED_API SystemBase
	{
	public:
		SystemBase(World* world) :m_world(world) {}
		virtual ~SystemBase() = default;
		virtual void Update(float deltaTime) = 0;

		World* GetWorld() { return m_world; }
		const World* GetWorld() const { return m_world; }
	protected:
		World* m_world;
	};
}


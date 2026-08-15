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
		SystemBase(const SystemBase&) = default;
		SystemBase& operator=(const SystemBase&) = default;
		virtual void Update(float deltaTime) = 0;

		void Pause() { m_paused = true; }
		void Resume() { m_paused = false; }
		bool IsPaused() const { return m_paused; }

		World* GetWorld() { return m_world; }
		const World* GetWorld() const { return m_world; }
	protected:
		World* m_world;
		bool m_paused = false;
	};
}


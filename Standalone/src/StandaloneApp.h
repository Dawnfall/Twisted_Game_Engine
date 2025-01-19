#pragma once

#include "AppCore.h"
#include "Twisted/AppBase.h"

namespace Twisted::Standalone
{
	class StandaloneApp : public AppBase
	{
	public:

		virtual void Run() override;

		void SetActiveWorld(SRef<World> world) { m_world = world; }
		SRef<World> GetActiveWorld() { return m_world; }
	
	private:
		void LoadResources();

		SRef<World> m_world = nullptr;
		bool Init();
	};
}
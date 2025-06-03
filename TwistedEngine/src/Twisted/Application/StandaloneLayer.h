#pragma once
#include "AppCore.h"
#include "Layer.h"
#include "Twisted/Game/World.h"

namespace Twisted
{
	class TWISTED_API StandaloneLayer :public Layer
	{
	public:
		StandaloneLayer(Application* app) :Layer(app) {}
		void OnInit()override;
		void OnFrame()override;
	private:
		SRef<World> m_world = nullptr;
	};
}
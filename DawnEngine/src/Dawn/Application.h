#pragma once

#include "Dawn/Game/GameCore.h"
#include "Rendering/RenderCore.h"

namespace Dawn
{
	class Application
	{
	public:
		Application();
		~Application();

		void Init();

		void Run();

	private:
		bool IsRunning();

		RenderCore m_renderCore;
		GameCore m_gameCore;
	};
}
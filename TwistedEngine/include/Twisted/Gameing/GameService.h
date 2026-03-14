#pragma once
#include "AppCore.h"
#include "Twisted/Application/Service.h"
#include "Twisted/Rendering/FrameBuffer.h"

#include "Twisted/Gameing/World.h"

#include "Twisted/Rendering/Renderers/GameRenderer.h"
#include "Utils/Event.h"

namespace Twisted
{
	class TWISTED_API GameService :public Service
	{
	public:
		GameService(Application* app) :Service(app) 
		{

		}


		void SetGameWorld(World& world);
		void CreateEmptyWorld();

		World* GameWorld = nullptr;
		GameRenderer renderer;

		unsigned int GameClearBits = 0;

	public:
		Event<World*> WorldChangeEvent;
	};
}
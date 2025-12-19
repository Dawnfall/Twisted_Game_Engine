#include "GameService.h"

namespace Twisted
{
	void GameService::SetGameWorld(World& world)
	{
		if (&world == GameWorld)
			return;

		if (GameWorld)
		{
			TObject::Destroy(GameWorld);
			GameWorld = nullptr;
		}

		GameWorld = &world;
		WorldChangeEvent.Invoke(GameWorld);
	}
	void GameService::CreateEmptyWorld()
	{
		if (GameWorld)
			TObject::Destroy(GameWorld);
		GameWorld = TObject::Create<World>("New world"); //m_app
	}
}
#include "GameService.h"
#include "Application/TObject.h"
#include "AssetsService.h"
#include "Application/Application.h"

namespace Twisted
{
	World* GameService::NewGameWorld(const std::string& name)
	{
		SetActiveWorld(TObject::Create<World>(name));
		return m_gameWorld;
	}

	World* GameService::LoadWorld(const fs::path& path)
	{
		return LoadWorld(m_app->GetService<AssetsService>()->GetUuid(path));
	}

	World* GameService::LoadWorld(AssetUuid uuid)
	{
		auto& assets = *m_app->GetService<AssetsService>();

		if (!uuid.IsValid() || !assets.Load(uuid))
			return nullptr;

		World* world = assets.GetObject<World>(uuid);
		if (!world)
			return nullptr;

		SetActiveWorld(world);
		return m_gameWorld;
	}

	void GameService::SetGameState(GameState newGameState)
	{
		if (m_gameWorldFrameInfo.gameState == newGameState)
			return;
		if (!m_gameWorld)
			return;

		m_gameWorldFrameInfo.gameState = newGameState;
		if (m_gameWorldFrameInfo.gameState == GameState::Stopped)
			m_gameWorldFrameInfo.Reset();

		GameStateChangeEvent.Invoke(m_gameWorldFrameInfo.gameState);
	}

	void GameService::SetActiveWorld(World* world)
	{
		if (m_gameWorld)
		{
			auto* assets = m_app->GetService<AssetsService>();
			if (assets && !assets->GetObjectUuid(m_gameWorld).IsValid())
				TObject::Destroy(m_gameWorld);
		}

		m_gameWorld = world;
		m_gameWorldFrameInfo = {};
		WorldChangeEvent.Invoke(m_gameWorld);
	}

	void GameService::OnInit()
	{
		m_timeService = m_app->GetService<TimeService>();
	}

	void GameService::OnFrame()
	{
		if (m_gameWorld && m_gameWorldFrameInfo.gameState == GameState::Playing)
			m_gameWorld->UpdateFrame(m_timeService->GetDeltaTime());
	}
}

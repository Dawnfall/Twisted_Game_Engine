#include "Twisted/Gameing/GameService.h"
#include "Twisted/TObject.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Application/Application.h"

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

	void GameService::SetGameWorld(World* world)
	{
		SetActiveWorld(world);
	}

	void GameService::Play()
	{
		if (m_gameState == GameState::Playing)
			return;
		m_gameState = GameState::Playing;
		GameStateChangeEvent.Invoke(m_gameState);
	}

	void GameService::Pause()
	{
		if (m_gameState != GameState::Playing)
			return;
		m_gameState = GameState::Paused;
		GameStateChangeEvent.Invoke(m_gameState);
	}

	void GameService::Stop()
	{
		if (m_gameState == GameState::Stopped)
			return;
		m_gameState = GameState::Stopped;
		GameStateChangeEvent.Invoke(m_gameState);
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
		WorldChangeEvent.Invoke(m_gameWorld);
	}
}

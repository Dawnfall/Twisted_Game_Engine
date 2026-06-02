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

	void GameService::SetPlaying(bool playing)
	{
		if (m_gameWorldFrameInfo.m_isPlaying == playing)
			return;
		if (!m_gameWorld)
			return;

		m_gameWorldFrameInfo.m_isPlaying = playing;
		if (!playing)
			m_gameWorldFrameInfo.Reset();
		// world restore is deferred — ExecuteRestore() runs at frame start before recording begins

		PlayingChangedEvent.Invoke(playing);
	}

	void GameService::SetPaused(bool paused)
	{
		if (m_gameWorldFrameInfo.m_isPaused == paused)
			return;

		m_gameWorldFrameInfo.m_isPaused = paused;
		PausedChangedEvent.Invoke(paused);
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

	void GameService::ExecuteRestore()
	{
		if (m_worldSnapshot.IsNull())
			return;

		BeforeWorldRestoreEvent.Invoke();
		m_gameWorld->Clear();
		YamlDeserialize<World>(*m_gameWorld, m_worldSnapshot);
		m_worldSnapshot = YAML::Node{};
	}

	void GameService::OnFirstFrame()
	{
		m_worldSnapshot = YamlSerialize<World>(*m_gameWorld);
	}

	void GameService::OnFrame()
	{
		if (!m_gameWorld || !m_gameWorldFrameInfo.m_isPlaying)
			return;

		if (m_gameWorldFrameInfo.m_firstFrame)
		{
			OnFirstFrame();
			m_gameWorldFrameInfo.m_firstFrame = false;
		}

		if (!m_gameWorldFrameInfo.m_isPaused)
			m_gameWorld->UpdateFrame(m_timeService->GetDeltaTime());
	}
}

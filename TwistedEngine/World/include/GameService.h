#pragma once
#include "AppCore.h"
#include "Application/Service.h"
#include "Application/TimeService.h"
#include "World.h"
#include "AssetUuid.h"
#include "Utils/Event.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Twisted
{
	class AssetsService;

	struct FrameInfo
	{
		bool m_isPlaying  = false;
		bool m_isPaused   = false;
		bool m_firstFrame = true;
		Time m_lastTime   = Time{};

		void Reset()
		{
			m_isPlaying  = false;
			m_isPaused   = false;
			m_firstFrame = true;
			m_lastTime   = Time{};
		}
	};

	class TWISTED_API GameService : public Service
	{
	public:
		GameService(Application* app,int priority) : Service(app, priority) {}
		~GameService() override = default;

		// Creates a new transient world with no file backing
		World* NewGameWorld(const std::string& name = "New World");
		void SetActiveWorld(World* world);

		// Loads a world asset through AssetsService
		World* LoadWorld(const fs::path& path);
		World* LoadWorld(AssetUuid uuid);

		World* GetGameWorld() { return m_gameWorld; }
		const FrameInfo& GetFrameInfo() const { return m_gameWorldFrameInfo; }

		bool IsPlaying() const { return m_gameWorldFrameInfo.m_isPlaying; }
		bool IsPaused()  const { return m_gameWorldFrameInfo.m_isPaused; }

		void SetPlaying(bool playing);
		void SetPaused(bool paused);

		bool HasPendingRestore() const { return !m_worldSnapshot.IsNull(); }
		void ExecuteRestore();

		void OnInit()  override;
		void OnFrame() override;

		Event<World*> WorldChangeEvent;
		Event<bool>   PlayingChangedEvent;
		Event<bool>   PausedChangedEvent;
		Event<>       BeforeWorldRestoreEvent;

	private:
		void OnFirstFrame();

		World*       m_gameWorld = nullptr;
		FrameInfo    m_gameWorldFrameInfo;
		TimeService* m_timeService;
		YAML::Node   m_worldSnapshot;
	};
}

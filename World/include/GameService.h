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

	enum class GameState
	{
		Stopped,
		Playing,
		Paused,
	};

	struct FrameInfo
	{
		GameState  gameState = GameState::Stopped;
		Time m_lastTime = Time{ };
		void Reset()
		{
			gameState = GameState::Stopped;
			m_lastTime = Time{};
		}
	};

	class TWISTED_API GameService : public Service
	{
	public:
		GameService(Application* app,int priority) : Service(app, priority) {}
		~GameService() = default;

		// Creates a new transient world with no file backing
		World* NewGameWorld(const std::string& name = "New World");
		void SetActiveWorld(World* world);

		// Loads a world asset through AssetsService
		World* LoadWorld(const fs::path& path);
		World* LoadWorld(AssetUuid uuid);

		World* GetGameWorld() { return m_gameWorld; }
		const FrameInfo& GetGameState() const { return m_gameWorldFrameInfo; }
		void SetGameState(GameState newGameState);

		void OnInit()override;
		void OnFrame()override;

		Event<World*>     WorldChangeEvent;
		Event<GameState>  GameStateChangeEvent;

	private:

		World*     m_gameWorld  = nullptr;
		FrameInfo m_gameWorldFrameInfo;
		TimeService* m_timeService;
	};
}

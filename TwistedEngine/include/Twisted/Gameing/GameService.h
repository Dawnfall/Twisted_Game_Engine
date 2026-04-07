#pragma once
#include "AppCore.h"
#include "Twisted/Application/Service.h"
#include "Twisted/Gameing/World.h"
#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Utils/Event.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Twisted
{
	class AssetsService;

	class TWISTED_API GameService : public Service
	{
	public:
		GameService(Application* app) : Service(app) {}
		~GameService();

		// Creates a new transient world (owned by this service)
		World* NewGameWorld(const std::string& name = "New World");

		// Loads a world from disk, GameService takes ownership
		World* LoadWorld(const fs::path& path);
		World* LoadWorld(const AssetUuid& uuid, const AssetsService& assets);

		// Sets an externally-owned world (e.g. runtime-created) as active
		void SetGameWorld(World* world);

		World* GetGameWorld() { return m_gameWorld; }
		AssetUuid GetGameWorldUuid() const { return m_gameWorldUuid; }

		Event<World*> WorldChangeEvent;

	private:
		void SetActiveWorld(World* world, bool owned);

		World* m_gameWorld = nullptr;
		bool m_ownsWorld = false;
		AssetUuid m_gameWorldUuid = AssetUuid::Invalid();
	};
}

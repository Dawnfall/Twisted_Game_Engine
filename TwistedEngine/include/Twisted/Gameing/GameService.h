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
		~GameService() = default;

		// Creates a new transient world with no file backing
		World* NewGameWorld(const std::string& name = "New World");

		// Loads a world asset through AssetsService
		World* LoadWorld(const fs::path& path);
		World* LoadWorld(AssetUuid uuid);

		// Sets an arbitrary world as active (caller manages lifetime if not in AssetsService)
		void SetGameWorld(World* world);

		World* GetGameWorld() { return m_gameWorld; }

		Event<World*> WorldChangeEvent;

	private:
		void SetActiveWorld(World* world);

		World* m_gameWorld = nullptr;
	};
}

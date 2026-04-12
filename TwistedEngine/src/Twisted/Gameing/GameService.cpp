#include "Twisted/Gameing/GameService.h"
#include "Twisted/TObject.h"
#include "Twisted/AssetsLayer/Importers/WorldImporter.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/AssetsLayer/AssetInfo.h"

namespace Twisted
{
	GameService::~GameService()
	{
		if (m_ownsWorld && m_gameWorld)
			TObject::Destroy(m_gameWorld);
	}

	World* GameService::NewGameWorld(const std::string& name)
	{
		m_gameWorldUuid = AssetUuid::Invalid();
		SetActiveWorld(TObject::Create<World>(name), true);
		return m_gameWorld;
	}

	World* GameService::LoadWorld(const fs::path& path)
	{
		m_gameWorldUuid = AssetUuid::Invalid();
		return LoadWorldFromPath(path);
	}

	void GameService::SetGameWorld(World* world)
	{
		m_gameWorldUuid = AssetUuid::Invalid();
		SetActiveWorld(world, false);
	}

	World* GameService::LoadWorldFromPath(const fs::path& path)
	{
		WorldImporter importer;
		auto objects = importer.Load(path);
		if (objects.empty())
			return nullptr;

		World* world = static_cast<World*>(objects[0].GetObj());
		SetActiveWorld(world, true);
		return m_gameWorld;
	}

	void GameService::SetActiveWorld(World* world, bool owned)
	{
		if (m_ownsWorld && m_gameWorld)
			TObject::Destroy(m_gameWorld);

		m_gameWorld = world;
		m_ownsWorld = owned;
		WorldChangeEvent.Invoke(m_gameWorld);
	}
}

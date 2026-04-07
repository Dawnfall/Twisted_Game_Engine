#include "Twisted/AssetsLayer/Importers/WorldImporter.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/TObject.h"
#include "Utils/WPtr.h"
#include "Utils/WPtrBase.h"
#include "Utils/YamlUtils.h"

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <memory>
#include <vector>

namespace Twisted
{
	std::vector<WPtrBase> WorldImporter::Load(const fs::path& path) const
	{
		YAML::Node data = YAML::LoadFile(path.string());

		WPtr<World> world(TObject::Create<World>(path.stem().string()));
		if (!world)
			return {};

		YamlDeserialize<World>(*world.get(), data);
		return { world };
	}

	void WorldImporter::HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const
	{
		(void)path;
		(void)objects;
		//TODO...
	}

	void WorldImporter::CreateNew(const fs::path& path) const
	{
		auto world = std::make_unique<World>(path.stem().string());
		if (!world)
			return;

		YAML::Node data = YamlSerialize<World>(*world.get());
		YamlUtils::saveNode(data, path, "Failed to create world asset!");
	}

	bool WorldImporter::Save(const fs::path& path, const std::vector<WPtrBase>& objects) const
	{
		if (objects.empty())
			return false;

		const World* world = static_cast<const World*>(objects[0].GetObj());
		if (!world)
			return false;

		YAML::Node data = YamlSerialize<World>(*world);
		YamlUtils::saveNode(data, path, "Failed to save world asset");
		return true;
	}
}

REGISTER_IMPORTER(WorldImporter)

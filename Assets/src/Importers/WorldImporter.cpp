#include "Importers/WorldImporter.h"
#include "AssetImporterRegistry.h"

#include "World.h"
#include "Application/TObject.h"
#include "Utils/WPtr.h"
#include "Utils/YamlUtils.h"

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <memory>

namespace Twisted
{
	SRef<FileImportedAsset> WorldImporter::Load(const fs::path& path) const
	{
		auto asset = std::make_shared<WorldAsset>(path);

		YAML::Node data = YAML::LoadFile(path.string());
		WPtr<World> world(TObject::Create<World>(path.stem().string()));
		YamlDeserialize<World>(*world.get(), data);

		asset->GetObjects().push_back(world);
		return asset;
	}

	void WorldImporter::HotReload(FileImportedAsset& asset) const
	{
		(void)asset;
		// TODO
	}

	void WorldImporter::CreateNew(const fs::path& path) const
	{
		auto world = std::make_unique<World>(path.stem().string());
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

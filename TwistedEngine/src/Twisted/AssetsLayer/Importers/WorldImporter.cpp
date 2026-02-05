#include "Twisted/AssetsLayer/Importers/WorldImporter.h"

#include "Twisted/Gameing/World.h"
#include "Utils/WPtr.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"
#include <vector>
#include "Utils/WPtrBase.h"
#include "Utils/YamlUtils.h"

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include "Twisted/TObject.h"
#include "Twisted/AssetsLayer/AssetInfo.h"
#include <memory>

namespace Twisted
{
	void WorldImporter::ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//BinSerializer buffer;
		//buffer.LoadFromFile(assetInfo.AssetPath);
		//assetInfo.GetAssetObjects()[""]= WPtr<World>(TObject::Create<World>(buffer));

		YAML::Node data = YAML::LoadFile(assetInfo.GetAssetPath().string());

		WPtr<World> world(TObject::Create<World>(assetInfo.GetAssetName()));
		if (!world)
			return;

		YamlDeserialize<World>(*world.get(), data);
		objects.emplace_back(world);
	}

	void WorldImporter::HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		(void)objects;
		(void)assetInfo;
		//TODO...
		//World* world = objects[""].GetObj()->static_as<World>();
		//world->Clear();
	}

	void WorldImporter::PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		(void)assetInfo;
		(void)objects;
		//BinSerializer buffer;
		//buffer.LoadFromFile(assetInfo.AssetPath);
		//objects[0]->static_as<World>()->Deserialize(buffer, assetsLayer);
	}

	void WorldImporter::CreateNewAsset(const fs::path& assetPath)const
	{
		//auto world = std::make_unique<World>();
		//BinSerializer buffer = world->Serialize();
		//buffer.SaveToFile(assetPath);

		auto world = std::make_unique<World>(assetPath.stem().string());

		if (!world)
			return;

		YAML::Node data = YamlSerialize<World>(*world.get());
		YamlUtils::saveNode(data, assetPath,"Failed to create world asset!");
	}

	bool WorldImporter::SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects)const
	{
		if (objects.empty())
			return false;

		const World* world = static_cast<const World*>(objects[0].GetObj());
		if (!world)
			return false;

		YAML::Node data = YamlSerialize<World>(*world);
		YamlUtils::saveNode(data, assetPath,"Failed to save world asset");

		return true;
	}

}

REGISTER_IMPORTER(WorldImporter)


#include "WorldImporter.h"

#include "Twisted/Gameing/World.h"
#include "Utils/WPtr.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Serialization/BinSerializer.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"
#include "Twisted/ObjectManager.h"
namespace Twisted
{
	void WorldImporter::ImportNew(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//BinSerializer buffer;
		//buffer.LoadFromFile(assetInfo.AssetPath);
		//assetInfo.GetAssetObjects()[""]= WPtr<World>(TObject::Create<World>(buffer));

		YAML::Node data = YAML::LoadFile(assetInfo.GetAssetPath().string());

		WPtr<World> world(ObjectManager::Create<World>(assetInfo.GetAssetName()));
		world->YamlDeserialize(data);

		objects.emplace_back(world);
	}

	void WorldImporter::HotReload(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		//TODO...
		//World* world = objects[""].GetObj()->static_as<World>();
		//world->Clear();
	}

	void WorldImporter::PostImport(AssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
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
		YAML::Node data = world->YamlSerialize();
		YamlUtils::saveNode(data, assetPath);
	}

	bool WorldImporter::SaveAsset(const fs::path& assetPath, const std::vector<WPtrBase>& objects)const
	{
		if (objects.empty())
			return false;

		const World* world = static_cast<const World*>(objects[0].GetObj());
		if (!world)
			return false;

		YAML::Node data = world->YamlSerialize();
		YamlUtils::saveNode(data, assetPath);

		return true;
	}

}

REGISTER_IMPORTER(WorldImporter)
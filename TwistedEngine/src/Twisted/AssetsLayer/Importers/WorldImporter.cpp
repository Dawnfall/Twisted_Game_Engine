#include "WorldImporter.h"

#include "Twisted/Gameing/World.h"
#include "Utils/WPtr.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Twisted/TwistedMacros.h"

namespace Twisted
{
	ObjectsPerAsset& WorldImporter::Import(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{
		if (objects.size() > 0)
		{
			World* world = objects[""].GetObj()->static_as<World>();
			world->Clear();
		}
		else
			objects[""] = WPtr<World>(TObject::Create<World>());// TODO... assetsLayer->GetApplication()
		return objects;
	}

	void WorldImporter::PostImport(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{
		BinSerializer buffer;
		buffer.LoadFromFile(assetInfo.AssetPath);
		//objects[0]->static_as<World>()->Deserialize(buffer, assetsLayer);
	}
}

REGISTER_IMPORTER(WorldImporter)

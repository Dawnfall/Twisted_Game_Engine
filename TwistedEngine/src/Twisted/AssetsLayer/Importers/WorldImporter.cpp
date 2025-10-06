#include "WorldImporter.h"

#include "Twisted/Gameing/World.h"
#include "Utils/WPtr.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "TwistedMacros.h"

namespace Twisted
{
	std::vector<TObject*>& WorldImporter::Import(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const
	{
		if (objects.size() > 0)
		{
			World* world = objects[0]->static_as<World>();
			world->Clear();
		}
		else
			objects.emplace_back(TObject::Create<World>(assetsLayer->GetApplication()));
		return objects;
	}

	void WorldImporter::PostImport(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const
	{
		BinSerializer buffer;
		buffer.LoadFromFile(assetInfo.AssetPath);
		objects[0]->static_as<World>()->Deserialize(buffer, assetsLayer);
	}
}

REGISTER_IMPORTER(WorldImporter)

#include "WorldImporter.h"

#include "Twisted/Gameing/World.h"

namespace Twisted
{
	AssetObjects WorldImporter::CreateObjects(AssetObjects& currObjects)const
	{
		if (currObjects.size()>0)
			currObjects[0] = ObjectManager::GetInstance().ReloadObject<World>(currObjects[0])->getID();
		else
			currObjects.emplace_back(ObjectManager::GetInstance().CreateObject<World>()->getID());
		return currObjects;
	}

	void WorldImporter::PostCreate(const AssetInfo& assetInfo, AssetObjects& objects, AssetsLayer* assetsLayer)const
	{
		BinSerializer buffer;
		buffer.LoadFromFile(assetInfo.AssetPath);

		World* world = ObjectManager::GetInstance().GetIdObject<World>(objects[0]);
		world->Deserialize(buffer,assetsLayer);
	}
}
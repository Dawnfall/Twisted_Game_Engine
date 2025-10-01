#include "TextureImporter.h"

#include <stb_image.h>
#include "Logger.h"
#include "Twisted/Rendering/Texture.h"

namespace Twisted
{
	AssetObjects TextureImporter::CreateObjects(AssetObjects& currObjects)const
	{
		if (currObjects.size()>0)
			currObjects[0] = ObjectManager::GetInstance().ReloadObject<Texture>(currObjects[0])->getID();
		else
			currObjects.emplace_back(ObjectManager::GetInstance().CreateObject<Texture>()->getID());
		return currObjects;
	}

	void TextureImporter::PostCreate(const AssetInfo& assetInfo, AssetObjects& objects, AssetsLayer* assetsLayer)const
	{
		TextureData texData;
		texData.Data = stbi_load(assetInfo.AssetPath.string().c_str(), &texData.Width, &texData.Height, &texData.Channels, 4); //must be 4 channels for now

		if (!texData.Data)
		{
			TWISTED_WARN("Failed to load texture: " + assetInfo.AssetPath.string() + " ; " + std::string(stbi_failure_reason()));
		}

		Texture* tex = ObjectManager::GetInstance().GetIdObject<Texture>(objects[0]);
		tex->Init(texData);

		stbi_image_free(texData.Data);
	}
}

#include "Twisted/AssetsLayer/AssetsRegistry.h"  
#include "TextureImporter.h"
#include "Twisted/TwistedMacros.h"  

#include <stb_image.h>
#include "Debug/Logger.h"
#include "Twisted/Rendering/Texture.h"
#include "Utils/WPtr.h"
#include "Twisted/TObject.h"

namespace Twisted
{
	TextureData LoadTextureData(const fs::path& assetPath)
	{
		TextureData texData;
		texData.Data = stbi_load(assetPath.string().c_str(), &texData.Width, &texData.Height, &texData.Channels, 4); //must be 4 channels for now

		if (!texData.Data)
		{
			TWISTED_WARN("Failed to load texture: " + assetPath.string() + " ; " + std::string(stbi_failure_reason()));
		}
		return texData;
	}

	ObjectsPerAsset& TextureImporter::Import(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{
		TextureData texData = LoadTextureData(assetInfo.AssetPath);
		if (!texData.Data)
			return objects;

		if (objects.size() > 0)
		{
			Texture* tex = objects[""].GetObj()->static_as<Texture>();
			tex->Clear();
			tex->Create(texData, TextureParams{});
		}
		else
		{
			objects[""]=WPtr<Texture>(TObject::Create<Texture>(texData, TextureParams{}));
		}

		stbi_image_free(texData.Data);

		return objects;
	}

	void TextureImporter::PostImport(const AssetInfo& assetInfo, ObjectsPerAsset& objects)const
	{

	}
}

REGISTER_IMPORTER(TextureImporter)

#include "Twisted/AssetsLayer/ImporterRegistry.h"  // <-- declares Twisted::ImporterRegistry
#include "TextureImporter.h"
#include "TwistedMacros.h"    // <-- defines REGISTER_IMPORTER

#include <stb_image.h>
#include "Logger.h"
#include "Twisted/Rendering/Texture.h"
#include "Utils/WPtr.h"
#include "Twisted/RegisterLayer/TObject.h"

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

	std::vector<TObject*>& TextureImporter::Import(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const
	{
		TextureData texData = LoadTextureData(assetInfo.AssetPath);
		if (!texData.Data)
			return objects;

		if (objects.size() > 0)
		{
			Texture* tex = objects[0]->static_as<Texture>();
			tex->Clear();
			tex->Create(texData, TextureParams{});
		}
		else
		{
			objects.emplace_back(TObject::Create<Texture>(texData, TextureParams{}));
		}

		stbi_image_free(texData.Data);

		return objects;
	}

	void TextureImporter::PostImport(const AssetInfo& assetInfo, std::vector<TObject*>& objects, AssetsLayer* assetsLayer)const
	{

	}
}

REGISTER_IMPORTER(TextureImporter)

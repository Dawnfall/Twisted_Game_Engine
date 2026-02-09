#include "Twisted/AssetsLayer/Importers/TextureImporter.h"

#include <stb_image.h>
#include "Debug/Logger.h"
#include "Twisted/Rendering/Texture.h"
#include "Utils/WPtr.h"
#include "Twisted/TObject.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"
#include <filesystem>
#include <Twisted/AssetsLayer/AssetInfo.h>
#include <string>
#include <Utils/WPtrBase.h>
#include <vector>

namespace Twisted
{
	static TextureData LoadTextureData(const fs::path& assetPath)
	{
		TextureData texData;
		stbi_set_flip_vertically_on_load(true);
		texData.Data = stbi_load(assetPath.string().c_str(), &texData.Width, &texData.Height, &texData.Channels, 4); //must be 4 channels for now

		if (!texData.Data)
		{
			TWISTED_WARN("Failed to load texture: {} ; {}", assetPath.string(), std::string(stbi_failure_reason()));
		}
		return texData;
	}

	void TextureImporter::ImportNew(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		TextureData texData = LoadTextureData(assetInfo.GetAssetPath());
		if (!texData.Data)
			return;

		WPtr<Texture> texture(TObject::Create<Texture>(assetInfo.GetAssetName()));
		texture->SetData(texData);

		objects.emplace_back(texture);
		stbi_image_free(texData.Data);
	}

	void TextureImporter::HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		(void)objects;
		(void)assetInfo;
		//TODO:...
		//TextureData texData = LoadTextureData(assetInfo.AssetPath);
		//if (!texData.Data)
		//	return;
		//Texture* tex = objects[""].GetObj()->static_as<Texture>();
		//tex->Clear();
		//tex->Create(texData, TextureParams{});
		//stbi_image_free(texData.Data);
	}

	void TextureImporter::PostImport(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
		(void)assetInfo;
		(void)objects;
	}
}

REGISTER_IMPORTER(TextureImporter)



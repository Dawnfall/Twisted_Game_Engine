#include "Twisted/AssetsLayer/AssetsLayer.h"  
#include "TextureImporter.h"

#include <stb_image.h>
#include "Debug/Logger.h"
#include "Twisted/Rendering/Texture.h"
#include "Utils/WPtr.h"
#include "Twisted/TObject.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

namespace Twisted
{


	TextureData LoadTextureData(const fs::path& assetPath)
	{
		TextureData texData;
		texData.Data = stbi_load(assetPath.string().c_str(), &texData.Width, &texData.Height, &texData.Channels, 4); //must be 4 channels for now

		if (!texData.Data)
		{
			TWISTED_WARN(std::format("Failed to load texture: {} ; {}" , assetPath.string(), std::string(stbi_failure_reason())));
		}
		return texData;
	}

	void TextureImporter::ImportNew(FileAssetInfo& assetInfo,std::vector<WPtrBase>& objects)const
	{
		TextureData texData = LoadTextureData(assetInfo.GetAssetPath());
		if (!texData.Data)
			return;

		WPtr<Texture> texture(TObject::Create<Texture>(assetInfo.GetAssetName(), TextureParams{}));
		texture->SetData(texData);

		objects.emplace_back(texture);
		stbi_image_free(texData.Data);
	}

	void TextureImporter::HotReload(FileAssetInfo& assetInfo, std::vector<WPtrBase>& objects)const
	{
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

	}
}

REGISTER_IMPORTER(TextureImporter)



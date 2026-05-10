#include "Importers/TextureImporter.h"
#include "AssetImporterRegistry.h"

#include "Texture.h"
#include "Application/TObject.h"
#include "Utils/WPtr.h"
#include "Debug/Logger.h"

#include <stb_image.h>

namespace Twisted
{
	SRef<FileImportedAsset> TextureImporter::Load(const fs::path& path) const
	{
		auto asset = std::make_shared<TextureAsset>(path);

		stbi_set_flip_vertically_on_load(true);
		TextureData texData;
		texData.Data = stbi_load(path.string().c_str(), &texData.Width, &texData.Height, &texData.Channels, 4);

		if (!texData.Data)
		{
			TWISTED_WARN("Failed to load texture: {} ; {}", path.string(), std::string(stbi_failure_reason()));
			return asset;
		}

		WPtr<Texture> texture(TObject::Create<Texture>(path.stem().string()));
		texture->SetData(texData, {});
		stbi_image_free(texData.Data);

		asset->GetObjects().push_back(texture);
		return asset;
	}

	void TextureImporter::HotReload(FileImportedAsset& asset) const
	{
		(void)asset;
		// TODO
	}
}

REGISTER_IMPORTER(TextureImporter)

#include "Twisted/AssetsLayer/Importers/TextureImporter.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

#include "Twisted/Rendering/Texture.h"
#include "Twisted/TObject.h"
#include "Utils/WPtr.h"
#include "Utils/WPtrBase.h"
#include "Debug/Logger.h"

#include <stb_image.h>
#include <filesystem>
#include <string>
#include <vector>

namespace Twisted
{
	static TextureData LoadTextureData(const fs::path& path)
	{
		TextureData texData;
		stbi_set_flip_vertically_on_load(true);
		texData.Data = stbi_load(path.string().c_str(), &texData.Width, &texData.Height, &texData.Channels, 4);

		if (!texData.Data)
			TWISTED_WARN("Failed to load texture: {} ; {}", path.string(), std::string(stbi_failure_reason()));

		return texData;
	}

	std::vector<WPtrBase> TextureImporter::Load(const fs::path& path) const
	{
		TextureData texData = LoadTextureData(path);
		if (!texData.Data)
			return {};

		WPtr<Texture> texture(TObject::Create<Texture>(path.stem().string()));
		texture->SetData(texData, {});
		stbi_image_free(texData.Data);

		return { texture };
	}

	void TextureImporter::HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const
	{
		(void)path;
		(void)objects;
		//TODO...
	}
}

REGISTER_IMPORTER(TextureImporter)

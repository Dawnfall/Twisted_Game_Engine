#include "Importers/ShaderImporter.h"
#include "AssetImporterRegistry.h"

#include "Shader.h"
#include "Application/TObject.h"
#include "Utils/WPtr.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"
#include "ShaderParser/ShaderData.h"

#include <format>

namespace Twisted
{
	SRef<FileImportedAsset> ShaderImporter::Load(const fs::path& path) const
	{
		auto asset = std::make_shared<ShaderAsset>(path);

		std::string source = Utils::ReadFileContent(path);
		ShaderData  data   = ProcessShaderFile(source, path.parent_path().string());
		if (!data.ok)
			TWISTED_WARN(std::format("Shader '{}': {}", path.string(), data.errorMessage));

		WPtr<Shader> shader(TObject::Create<Shader>(path.stem().string()));
		shader->SetData(data);

		asset->GetObjects().push_back(shader);
		return asset;
	}

	void ShaderImporter::HotReload(FileImportedAsset& asset) const
	{
		(void)asset;
		// TODO
	}
}

REGISTER_IMPORTER(ShaderImporter)

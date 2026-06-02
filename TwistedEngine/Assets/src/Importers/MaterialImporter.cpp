#include "Importers/MaterialImporter.h"
#include "AssetImporterRegistry.h"
#include "AssetsService.h"
#include "Application/Application.h"

#include "Material.h"
#include "Shader.h"
#include "Application/TObject.h"
#include "Utils/WPtr.h"
#include "Utils/YamlUtils.h"
#include "ShaderParser/Shaders/Default_Shader.h"

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <memory>

namespace Twisted
{
	SRef<FileImportedAsset> MaterialImporter::Load(const fs::path& path) const
	{
		auto asset = std::make_shared<MaterialAsset>(path);

		WPtr<Material> mat(TObject::Create<Material>(path.stem().string()));

		YAML::Node buffer = YAML::LoadFile(path.string());
		YamlDeserialize<Material>(*mat, buffer);

		asset->GetObjects().push_back(mat);
		return asset;
	}

	void MaterialImporter::HotReload(FileImportedAsset& asset) const
	{
		(void)asset;
		// TODO
	}

	void MaterialImporter::CreateNew(const fs::path& path) const
	{
		auto material = std::make_unique<Material>(path.stem().string());

		AssetUuid defaultShaderUuid{ DefaultShaderUUID.first, DefaultShaderUUID.second };
		Shader* defaultShader = Application::GetInstance().GetService<AssetsService>()->GetObject<Shader>(defaultShaderUuid);
		if (defaultShader)
			material->SetShader(defaultShader);

		YAML::Node data = YamlSerialize<Material>(*material);
		YamlUtils::saveNode(data, path, "Failed to create material asset!");
	}

	bool MaterialImporter::Save(const fs::path& path, const std::vector<WPtrBase>& objects) const
	{
		if (objects.empty())
			return false;

		const Material* mat = static_cast<const Material*>(objects[0].GetObj());
		if (!mat)
			return false;

		YAML::Node data = YamlSerialize<Material>(*mat);
		YamlUtils::saveNode(data, path, "Cannot save material");
		return true;
	}
}

REGISTER_IMPORTER(MaterialImporter)

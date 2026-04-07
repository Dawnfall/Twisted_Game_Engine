#include "Twisted/AssetsLayer/Importers/MaterialImporter.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

#include "Twisted/Rendering/Material.h"
#include "Twisted/TObject.h"
#include "Utils/WPtr.h"
#include "Utils/WPtrBase.h"
#include "Utils/YamlUtils.h"

#include <filesystem>
#include <memory>
#include <vector>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

namespace Twisted
{
	std::vector<WPtrBase> MaterialImporter::Load(const fs::path& path) const
	{
		WPtr<Material> mat(TObject::Create<Material>(path.stem().string()));
		return { mat };
	}

	void MaterialImporter::PostLoad(const fs::path& path, std::vector<WPtrBase>& objects) const
	{
		if (objects.empty())
			return;

		Material* mat = static_cast<Material*>(objects[0].GetObj());
		if (!mat)
			return;

		YAML::Node buffer = YAML::LoadFile(path.string());
		YamlDeserialize<Material>(*mat, buffer);
	}

	void MaterialImporter::HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const
	{
		(void)path;
		(void)objects;
		//TODO...
	}

	void MaterialImporter::CreateNew(const fs::path& path) const
	{
		auto material = std::make_unique<Material>(path.stem().string());
		if (!material)
			return;

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

#include "Twisted/AssetsLayer/Importers/ModelImporter.h"
#include "Twisted/AssetsLayer/AssetImporterRegistry.h"

#include "Utils/GlmUtils.h"
#include "Data/Vertex.h"
#include "Utils/WPtrBase.h"

#include <rapidobj/rapidobj.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>

namespace Twisted
{
	std::vector<WPtrBase> ModelImporter::Load(const fs::path& path) const
	{
		(void)path;
		//TODO...
		return {};
	}

	void ModelImporter::HotReload(const fs::path& path, std::vector<WPtrBase>& objects) const
	{
		(void)path;
		(void)objects;
		//TODO...
	}
}

REGISTER_IMPORTER(ModelImporter)

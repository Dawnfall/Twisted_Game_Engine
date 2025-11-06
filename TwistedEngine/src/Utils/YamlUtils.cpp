#include "YamlUtils.h"
#include <fstream>

#include "Twisted/AssetsLayer/AssetsLayer.h"

namespace YamlUtils
{
	bool saveNode(const YAML::Node& node, const std::filesystem::path& path)
	{
		try
		{
			std::ofstream fout(path);
			fout << node;
			fout.close();
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	YAML::Node encodeTObject(const Twisted::TObject* obj)
	{
		YAML::Node node;
		if (!obj)
			return node;

		node["name"] = obj->GetName();
		node["uuid"] = Twisted::AssetsLayer::GetInstance().GetObjectAsset(obj);
		return node;
	}

	Twisted::TObject* decodeTObject(const YAML::Node& node)
	{
		std::string name = node["name"].as<std::string>("");
		Twisted::AssetUuid uuid = node["uuid"].as<Twisted::AssetUuid>(Twisted::AssetUuid::Invalid());

		return Twisted::AssetsLayer::GetInstance().GetAssetObject(uuid, name);

	}
}
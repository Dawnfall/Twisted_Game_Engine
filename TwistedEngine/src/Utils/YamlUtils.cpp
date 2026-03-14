#include "Utils/YamlUtils.h"
#include <fstream>

#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Application/Application.h"

namespace YamlUtils
{
	bool saveNode(const YAML::Node& node, const std::filesystem::path& path, const std::string& errorMsg)
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
			TWISTED_WARN(errorMsg);
			return false;
		}
	}

	bool loadNode(const std::filesystem::path& path, YAML::Node& node, const std::string& errorMsg)
	{
		try
		{
			node = YAML::LoadFile(path.string());
			return true;
		}
		catch (...)
		{
			TWISTED_ERROR(errorMsg);
			return false;
		}
	}


	YAML::Node encodeTObject(const Twisted::TObject* obj)
	{
		YAML::Node node;
		if (!obj)
		{
			node[Twisted::NAME_SER_KEY] = "";
			node[Twisted::UUID_SER_KEY] = Twisted::AssetUuid::Invalid();
		}
		else
		{
			node[Twisted::NAME_SER_KEY] = obj->GetName();
			node[Twisted::UUID_SER_KEY] =Twisted::Application::GetInstance().GetService<Twisted::AssetsService>()->GetObjectAssetInfo(obj)->GetUuid();
		}
		return node;
	}

	Twisted::TObject* decodeTObject(const YAML::Node& node)
	{
		std::string name = node["name"].as<std::string>("");
		Twisted::AssetUuid uuid = node["uuid"].as<Twisted::AssetUuid>(Twisted::AssetUuid::Invalid());

		return Twisted::Application::GetInstance().GetService<Twisted::AssetsService>()->GetAssetObject(uuid, name);

	}
}


#include "AssetEncoder.h"
#include "AssetUuidYaml.h"
#include "AssetsService.h"
#include "Constants.h"
#include "Application/Application.h"
#include "Application/TObject.h"

namespace Twisted
{
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
			node[Twisted::UUID_SER_KEY] = Twisted::Application::GetInstance().GetService<Twisted::AssetsService>()->GetObjectUuid(obj);
		}
		return node;
	}

	Twisted::TObject* decodeTObject(const YAML::Node& node)
	{
		std::string name = node["name"].as<std::string>("");
		Twisted::AssetUuid uuid = node["uuid"].as<Twisted::AssetUuid>(Twisted::AssetUuid::Invalid());

		auto* assets = Twisted::Application::GetInstance().GetService<Twisted::AssetsService>();

		if (uuid.IsValid() && !assets->IsLoaded(uuid))
			assets->Load(uuid);

		return assets->GetObject(uuid, name);
	}
}
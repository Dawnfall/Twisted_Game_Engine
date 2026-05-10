#include "ProjectConfig.h"
#include "AssetUuidYaml.h"
#include "Debug/Logger.h"
#include "Utils/YamlUtils.h"

#include <yaml-cpp/yaml.h>

namespace Twisted
{
	void ProjectConfig::Load(const fs::path& path)
	{
		m_path = path;

		YAML::Node node;
		YamlUtils::loadNode(path, node, "Failed to load project config!");

		lastWorld    = node["last_world"].as<AssetUuid>(AssetUuid::Invalid());
		imguiIni     = node["imgui_ini"].as<std::string>("");
		ambientLight = node["ambient_light"].as<Vec4f>(Vec4f(1.0f, 1.0f, 1.0f, 0.05f));

		panelStates.clear();
		if (node["panels"])
		{
			for (const auto& p : node["panels"])
				panelStates.push_back({ p["name"].as<std::string>(), p["showing"].as<bool>(true) });
		}

		TWISTED_INFO("ProjectConfig loaded: ", path.string());
	}

	void ProjectConfig::Save() const
	{
		YAML::Node node;

		node["last_world"]    = lastWorld;
		node["ambient_light"] = ambientLight;

		if (!imguiIni.empty())
			node["imgui_ini"] = imguiIni;

		if (!panelStates.empty())
		{
			YAML::Node panelsNode;
			for (const auto& ps : panelStates)
			{
				YAML::Node p;
				p["name"] = ps.name;
				p["showing"] = ps.showing;
				panelsNode.push_back(p);
			}
			node["panels"] = panelsNode;
		}

		YamlUtils::saveNode(node, m_path, "Failed to save project config!");
		TWISTED_INFO("ProjectConfig saved: ", m_path.string());
	}
}

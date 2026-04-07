#include "LayoutManager.h"

#include "EditorApp/EditorRegistry.h"
#include "Utils/YamlUtils.h"

#include <imgui.h>
#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace Twisted::Editor
{
	LayoutManager& LayoutManager::GetInstance()
	{
		static LayoutManager instance;
		return instance;
	}

	void LayoutManager::SaveLayout(const std::filesystem::path& path)
	{
		YAML::Node root;

		size_t iniSize = 0;
		const char* iniData = ImGui::SaveIniSettingsToMemory(&iniSize);
		root["imgui_ini"] = std::string(iniData, iniSize);

		YAML::Node panelsNode;
		for (const auto& panel : EditorRegistry::GetInstance().m_panels)
		{
			YAML::Node p;
			p["name"] = panel->GetName();
			p["showing"] = panel->IsShowing;
			panelsNode.push_back(p);
		}
		root["panels"] = panelsNode;

		YamlUtils::saveNode(root, path, "Failed to save layout: " + path.string());
	}

	bool LayoutManager::LoadLayout(const std::filesystem::path& path)
	{
		YAML::Node root;
		if (!YamlUtils::loadNode(path, root, "Failed to load layout: " + path.string()))
			return false;

		if (root["imgui_ini"])
		{
			const std::string ini = root["imgui_ini"].as<std::string>();
			ImGui::LoadIniSettingsFromMemory(ini.c_str(), ini.size());
		}

		if (root["panels"])
		{
			auto& panels = EditorRegistry::GetInstance().m_panels;
			for (const auto& pNode : root["panels"])
			{
				const std::string pname = pNode["name"].as<std::string>();
				const bool showing = pNode["showing"].as<bool>();
				for (auto& panel : panels)
				{
					if (panel->GetName() == pname)
					{
						panel->IsShowing = showing;
						break;
					}
				}
			}
		}

		return true;
	}
}

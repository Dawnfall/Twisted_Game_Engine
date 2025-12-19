#pragma once

#include "Utils/FileUtils.h"
#include "Utils/Utils.h"
#include "Utils/YamlUtils.h"

#include <vector>
#include <string>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>

namespace Twisted::Editor
{
	//to load data at project selection phase
	class LoadupConfig
	{
	public:
		LoadupConfig(const fs::path& path) 
		{
			ConfigPath = path;
			Node = YAML::LoadFile(path.string());
		}

		std::vector<std::string> GetRecentProjects()
		{
			return Node["recent"].as<std::vector<std::string>>();
		}

		void AddLatest(const std::string& latestProject)
		{
			auto recentProjects = GetRecentProjects();
			Utils::RemoveElement<std::string>(recentProjects, latestProject);
			recentProjects.insert(recentProjects.begin(), latestProject);
			if (recentProjects.size() > 5)
				recentProjects.erase(recentProjects.begin() + 5, recentProjects.end());
			Node["recent"] = recentProjects;
		}

		void RemoveEntry(const std::string& entry)
		{
			auto recentProjects = GetRecentProjects();
			Utils::RemoveElement<std::string>(recentProjects, entry);
			recentProjects.erase(std::find(recentProjects.begin(), recentProjects.end(), entry));
			Node["recent"] = recentProjects;
		}

		void Save()
		{
			YamlUtils::saveNode(Node,ConfigPath.string());
		}

		fs::path ConfigPath;
		YAML::Node Node;
	};
}

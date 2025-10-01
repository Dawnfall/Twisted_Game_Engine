#pragma once

#include "Utils/FileUtils.h"
#include "Utils/Utils.h"
#include "Twisted/Data/YamlFile.h"

#include <vector>
#include <string>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>

namespace Twisted::Editor
{
	//inline static const fs::path loadupConfigPath = "F:/Programiranje/Test/TwistedEditor/loadupConfig.editor";

	//to load data at project selection phase
	class LoadupConfig
	{
	public:
		LoadupConfig() :
			File("F:/Programiranje/Test/TwistedEditor/loadupConfig.editor")
		{

		}

		std::vector<std::string> GetRecentProjects()
		{
			return File.GetData<std::vector<std::string>>("recent", {});
		}

		void AddLatest(const std::string& latestProject)
		{
			auto recentProjects = GetRecentProjects();
			Utils::RemoveElement<std::string>(recentProjects, latestProject);
			recentProjects.insert(recentProjects.begin(), latestProject);
			if (recentProjects.size() > 5)
				recentProjects.erase(recentProjects.begin() + 5, recentProjects.end());
			File.SetData<std::vector<std::string>>("recent", recentProjects);
		}

		void RemoveEntry(const std::string& entry)
		{
			auto recentProjects = GetRecentProjects();
			Utils::RemoveElement<std::string>(recentProjects, entry);
			recentProjects.erase(std::find(recentProjects.begin(), recentProjects.end(), entry));
			File.SetData<std::vector<std::string>>("recent", recentProjects);
		}

		YamlFile File;
	};
}
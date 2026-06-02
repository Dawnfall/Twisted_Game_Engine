#pragma once
#include "Utils/GlmUtils.h"
#include "Utils/FileUtils.h"
#include "Utils/YamlUtils.h"
#include "Utils/Utils.h"

#include "Debug/Logger.h"

#include "EditorConstants.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

namespace Twisted::Editor
{
	class EditorConfig
	{
	public:
		static EditorConfig& GetInstance()
		{
			static EditorConfig instance;
			return instance;
		}

		EditorConfig(const EditorConfig&) = delete;
		EditorConfig& operator=(const EditorConfig&) = delete;

		YAML::Node m_rootNode;

		void LoadConfig()
		{
			YamlUtils::loadNode(Constants::CONFIG_FILE_PATH.string(), m_rootNode, "Cannot load editor config!");
		}

		void SaveConfig()
		{
			YamlUtils::saveNode(m_rootNode, Constants::CONFIG_FILE_PATH, "Error writing Config File!");
		}

		// Window
		Vec2i GetWindowSize()
		{
			return m_rootNode[Constants::WIN_SIZE_NAME].as<Vec2i>(Constants::WINDOW_DEFAULT_SIZE);
		}
		Vec2i GetWindowPos()
		{
			return m_rootNode[Constants::WIN_POS_NAME].as<Vec2i>(Constants::WINDOW_DEFAULT_POS);
		}
		bool GetWindowMaximized()
		{
			return m_rootNode[Constants::WIN_MAXIMIZED_NAME].as<bool>(false);
		}
		void SetWindowSize(Vec2i size)
		{
			m_rootNode[Constants::WIN_SIZE_NAME] = size;
		}
		void SetWindowPos(Vec2i pos)
		{
			m_rootNode[Constants::WIN_POS_NAME] = pos;
		}
		void SetWindowMaximized(bool maximized)
		{
			m_rootNode[Constants::WIN_MAXIMIZED_NAME] = maximized;
		}

		// Recent projects
		std::vector<std::string> GetRecentProjects()
		{
			return m_rootNode[Constants::RECENT_PROJECTS_NAME].as<std::vector<std::string>>(std::vector<std::string>{});
		}
		void AddLatest(const std::string& project)
		{
			auto recent = GetRecentProjects();
			Utils::RemoveElement<std::string>(recent, project);
			recent.insert(recent.begin(), project);
			if (recent.size() > 5)
				recent.erase(recent.begin() + 5, recent.end());
			m_rootNode[Constants::RECENT_PROJECTS_NAME] = recent;
		}
		void RemoveEntry(const std::string& project)
		{
			auto recent = GetRecentProjects();
			Utils::RemoveElement<std::string>(recent, project);
			m_rootNode[Constants::RECENT_PROJECTS_NAME] = recent;
		}

	private:
		EditorConfig() = default;
	};
}

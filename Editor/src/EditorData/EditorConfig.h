#pragma once
#include "Utils/GlmUtils.h"
#include "Utils/FileUtils.h"
#include "Utils/YamlUtils.h"

#include "Debug/Logger.h"

#include "EditorConstants.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <fstream>

namespace Twisted::Editor
{
	struct EditorConfig
	{
		YAML::Node m_rootNode;

		void LoadConfig()
		{
			YamlUtils::loadNode(Constants::CONFIG_FILE_PATH.string(), m_rootNode, "Cannot load editor config!");
		}

		void SaveConfig()
		{
			YamlUtils::saveNode(m_rootNode, Constants::CONFIG_FILE_PATH, "Error writing Config File!");
		}

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
	};
}

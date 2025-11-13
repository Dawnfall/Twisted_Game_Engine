#pragma once
#include "Utils/GlmUtils.h"
#include "Utils/FileUtils.h"
#include "Debug/Logger.h"

#include "EditorConstants.h"

#include <yaml-cpp/yaml.h>
#include <string>
#include <filesystem>
#include <fstream>

namespace Twisted::Editor
{
	struct EditorConfig
	{
		YAML::Node m_rootNode;

		void LoadConfig()
		{
			if (Utils::IsExisting(Constants::CONFIG_FILE_PATH))
				m_rootNode = YAML::LoadFile(Constants::CONFIG_FILE_PATH.string());
		}

		void SaveConfig()
		{
			try
			{
				std::ofstream fout(Constants::CONFIG_FILE_PATH);
				fout << m_rootNode;
				fout.close();
			}
			catch (...)
			{
				TWISTED_WARN("Error writing Config File!");
			}
		}

		Vec2i GetWindowSize()
		{
			return m_rootNode[Constants::WIN_SIZE_NAME].as<Vec2i>(Constants::WINDOW_DEFAULT_SIZE);
		}
		Vec2i GetWindowPos()
		{
			return m_rootNode[Constants::WIN_POS_NAME].as<Vec2i>(Constants::WINDOW_DEFAULT_POS);
		}

		void SetWindowSize(Vec2i size)
		{
			m_rootNode[Constants::WIN_SIZE_NAME] = size;
		}
		void SetWindowPos(Vec2i pos)
		{
			m_rootNode[Constants::WIN_POS_NAME] = pos;
		}
	};
}

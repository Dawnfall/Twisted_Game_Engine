#pragma once

#include "Twisted/AssetsLayer/AssetUuid.h"
#include "Utils/YamlUtils.h"
#include "Utils/FileUtils.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace Twisted::Editor
{
	class ProjectConfig
	{
	public:
		ProjectConfig(const fs::path& projectFilePath) : m_path(projectFilePath)
		{
			YamlUtils::loadNode(projectFilePath, m_node, "Failed to load project config!");
		}

		AssetUuid GetLastWorldUuid() const
		{
			return m_node["last_world"].as<AssetUuid>(AssetUuid::Invalid());
		}

		void SetLastWorld(const AssetUuid& uuid)
		{
			m_node["last_world"] = uuid;
		}

		void ClearLastWorld()
		{
			m_node.remove("last_world");
		}

		void Save()
		{
			YamlUtils::saveNode(m_node, m_path, "Failed to save project config!");
		}

	private:
		fs::path m_path;
		YAML::Node m_node;
	};
}

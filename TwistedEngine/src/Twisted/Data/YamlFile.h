#pragma once
#include "AppCore.h"

#include "Utils/FileUtils.h"
#include "Utils/GlmUtils.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>

//namespace YAML {
//	template<>
//	struct convert<std::vector<std::string>> {
//		static Node encode(const std::vector<std::string>& rhs) {
//			Node node(NodeType::Sequence);
//			for (const auto& s : rhs)
//				node.push_back(s);
//			return node;
//		}
//
//		static bool decode(const Node& node, std::vector<std::string>& rhs) {
//			if (!node.IsSequence())
//				return false;
//			rhs.clear();
//			for (auto it = node.begin(); it != node.end(); ++it)
//				rhs.push_back(it->as<std::string>());
//			return true;
//		}
//	};
//}

namespace Twisted
{
	class TWISTED_API YamlFile
	{
	public:
		YamlFile(const std::filesystem::path& path);
		bool Load();
		bool Save()const;

		template<typename T>
		T GetData(const std::string& key, T defaultValue)
		{
			return m_rootNode[key].as<T>(defaultValue);
		}

		template<typename T>
		void SetData(const std::string& key, const T& value)
		{
			m_rootNode[key] = value;
		}

		const std::filesystem::path& GetPath()const { return m_path; }
	private:
		std::filesystem::path m_path;
		YAML::Node m_rootNode;
	};
}

// Specialize YAML::convert for Vec2i
namespace YAML
{
	template<>
	struct convert<Vec2i>
	{
		static Node encode(const Vec2i& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, Vec2i& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			return true;
		}
	};
}
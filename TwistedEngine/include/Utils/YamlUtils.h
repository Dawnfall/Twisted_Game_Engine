#pragma once
#include "AppCore.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace Twisted
{
	template<typename T>
	YAML::Node YamlSerialize([[maybe_unused]] const T& object) { return YAML::Node{}; }
	template<typename T>
	void YamlDeserialize([[maybe_unused]] T& object, [[maybe_unused]] const YAML::Node& node) {}
}

namespace Twisted
{
	class TObject;
}

namespace YamlUtils
{
	bool TWISTED_API saveNode(const YAML::Node& node, const std::filesystem::path& path);

	TWISTED_API YAML::Node encodeTObject(const Twisted::TObject* obj);
	TWISTED_API Twisted::TObject* decodeTObject(const YAML::Node& node);
}


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


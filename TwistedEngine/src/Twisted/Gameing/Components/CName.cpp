#include "CName.h"

namespace Twisted
{
	void CName::Serialize(BinSerializer& buffer)const
	{
		buffer.Write<std::string>(m_name, nullptr);
	}
	void CName::Deserialize(BinSerializer& buffer)
	{
		m_name = buffer.Read<std::string>(nullptr);
	}

	YAML::Node CName::YamlSerialize() const
	{
		YAML::Node node;

		node["name"] = m_name;

		return node;
	}
	void CName::YamlDeserialize(const YAML::Node& node)
	{
		m_name = node["name"].as<std::string>();
	}
}



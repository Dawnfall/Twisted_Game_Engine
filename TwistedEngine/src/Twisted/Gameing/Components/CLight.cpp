#include "CLight.h"

namespace Twisted
{
	void CLight::Serialize(BinSerializer& buffer)const
	{
		buffer.Write<LightType>(m_type, nullptr);
	}
	void CLight::Deserialize(BinSerializer& buffer)
	{
		m_type = buffer.Read<LightType>(nullptr);
	}

	YAML::Node CLight::YamlSerialize() const
	{
		YAML::Node node;

		node["type"] = m_type;

		return node;
	}
	void CLight::YamlDeserialize(const YAML::Node& node)
	{
		m_type = node["type"].as<LightType>();
	}
}


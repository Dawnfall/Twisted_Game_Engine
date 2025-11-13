#pragma once

#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/Gameing/WorldRegistry.h"

namespace Twisted
{
	enum class TWISTED_API LightType
	{
		DIRECTIONAL = 0,
		POINT = 1
	};

	class TWISTED_API CLight :public AComponent
	{
	public:
		CLight(Entity entity) :
			AComponent(entity)
		{
		}

		//LightType GetType()const { return m_type; }
		//void SetType(LightType lightType) { m_type = lightType; }

		void Serialize(BinSerializer& buffer)const override;
		void Deserialize(BinSerializer& buffer)override;

		YAML::Node YamlSerialize() const override;
		void YamlDeserialize(const YAML::Node& node) override;

	private:

		//LightType m_type;
	};
}

namespace YAML
{
	template<>
	struct convert<Twisted::LightType>
	{
		static Node encode(const Twisted::LightType& rhs)
		{
			Node node;
			switch (rhs)
			{
			case Twisted::LightType::DIRECTIONAL: node = "Directional"; break;
			case Twisted::LightType::POINT:       node = "Point";       break;
			default:
				throw std::runtime_error("YAML::convert<LightType>::encode: Unknown LightType enum value");
			}
			return node;
		}

		static bool decode(const Node& node, Twisted::LightType& rhs)
		{
			if (!node || !node.IsScalar())
				throw std::runtime_error("YAML::convert<LightType>::decode: Invalid or missing YAML node");

			const std::string value = node.as<std::string>();

			if (value == "Directional") { rhs = Twisted::LightType::DIRECTIONAL; return true; }
			if (value == "Point") { rhs = Twisted::LightType::POINT;       return true; }

			throw std::runtime_error("YAML::convert<LightType>::decode: Unknown LightType string: " + value);
		}
	};
} // namespace YAML


REGISTER_COMPONENT(CLight, "CLight");



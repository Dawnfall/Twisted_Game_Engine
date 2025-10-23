#include "CLight.h"
#include "Twisted/TwistedMacros.h"

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
}

REGISTER_COMPONENT(CLight, "CLight");
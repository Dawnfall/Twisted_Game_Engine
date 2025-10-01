#include "CLight.h"

namespace Twisted
{
	void CLight::Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const
	{
		//buffer.Write<LightType>(m_type);
	}
	void CLight::Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)
	{
		//m_type = buffer.Read<LightType>();
	}
}
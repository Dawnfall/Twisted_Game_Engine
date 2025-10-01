#include "CName.h"

namespace Twisted
{
	void CName::Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const
	{
		buffer.Write<std::string>(m_name);
	}
	void CName::Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)
	{
		m_name = buffer.Read<std::string>();
	}
}
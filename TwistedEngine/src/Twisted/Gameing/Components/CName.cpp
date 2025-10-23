#include "CName.h"
#include "Twisted/TwistedMacros.h"

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
}


REGISTER_COMPONENT(CName,"CName");
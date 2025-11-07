#include "ObjectID.h"
#include "ObjectManager.h"

namespace Twisted
{
	bool ObjectID::IsValid() const
	{
		return ObjectManager::IsValidID(*this);
	}

	std::string ObjectID::ToString()const
	{
		return std::to_string(m_id);
	}
}
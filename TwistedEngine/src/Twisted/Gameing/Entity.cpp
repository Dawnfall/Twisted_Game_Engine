#include "Twisted/Gameing/Entity.h"

#include "Twisted/Gameing/World.h"

namespace Twisted
{
	bool Entity::IsValid()const { return m_world && m_world->GetRegistry().valid(m_id); }




}


#pragma once
#include "Twisted/Gameing/ManagerBase.h"
#include "Twisted/Gameing/Entity.h"

namespace Twisted
{
	class CameraManager:public ManagerBase
	{
	public:
		EntityID MainCamera = NullEntity;
	};
}
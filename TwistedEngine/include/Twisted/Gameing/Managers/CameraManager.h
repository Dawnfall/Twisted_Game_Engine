#pragma once
#include "Twisted/Gameing/ManagerBase.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/Gameing/WorldRegistry.h"

namespace Twisted
{
	class CameraManager:public ManagerBase
	{
	public:
		EntityID MainCamera = NullEntity;
	};
}

REGISTER_MANAGER(CameraManager, "CameraManager");
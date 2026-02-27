#pragma once
#include "AppCore.h"
#include "Twisted/Gameing/ManagerBase.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/Gameing/WorldRegistry.h"

namespace Twisted
{
	class CameraComponent;
	class TWISTED_API CameraManager:public ManagerBase
	{
	public:
		CameraManager(World* world) :ManagerBase(world) {}
		CameraComponent* GetMainCamera();
		void SetAsMainCamera(CameraComponent* cam);

	private:
		EntityID m_mainCamera = NullEntity;
	};
}

REGISTER_MANAGER(CameraManager, "CameraManager");
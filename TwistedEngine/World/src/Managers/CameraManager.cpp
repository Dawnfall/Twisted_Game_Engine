#include "Managers/CameraManager.h"
#include "Components/CCamera.h"
#include "World.h"

namespace Twisted
{
	void CameraManager::SetAsMainCamera(CameraComponent* cam)
	{
		if (!cam)
			m_mainCamera = NullEntity;
		else
			m_mainCamera = cam->GetID();
	}

	CameraComponent* CameraManager::GetMainCamera()
	{
		return world->TryGetComponent<CameraComponent>(m_mainCamera);
	}
}

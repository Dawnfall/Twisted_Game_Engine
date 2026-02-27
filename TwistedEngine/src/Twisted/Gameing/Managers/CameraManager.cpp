#include "Twisted/Gameing/Managers/CameraManager.h"
#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/World.h"

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

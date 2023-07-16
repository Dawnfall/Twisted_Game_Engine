#include "CCamera.h"

namespace Dawn
{
	glm::mat4 CCamera::GetViewMatrix()const
	{
		return glm::lookAt(m_gameEntity->_transform.GetWorldPosition(), m_gameEntity->_transform.GetWorldForward(), m_gameEntity->_transform.GetWorldUp());
	}

	glm::mat4 CCamera::GetProjectionMatrix()const
	{
		if (m_cameraType == CameraProjectionType::ORTOGRAPHIC)
			return glm::ortho(m_leftEdge, m_rightEdge, m_botEdge, m_topEdge, m_nearPlane, m_farPlane);
		return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
	}
}
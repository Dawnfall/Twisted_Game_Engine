#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "CCamera.h"

namespace Dawn
{
	//glm::mat4 CCamera::GetViewMatrix(const World& world)const
	//{
	//	Transform transform = world.GetComponent<Transform>(m_entityID);
	//	return glm::lookAt(transform.GetWorldPosition(), transform.GetWorldForward(), transform.GetWorldUp());
	//}

	//glm::mat4 CCamera::GetProjectionMatrix()const
	//{
	//	if (m_cameraType == CameraProjectionType::ORTOGRAPHIC)
	//		return glm::ortho(m_leftEdge, m_rightEdge, m_botEdge, m_topEdge, m_nearPlane, m_farPlane);
	//	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
	//}
}
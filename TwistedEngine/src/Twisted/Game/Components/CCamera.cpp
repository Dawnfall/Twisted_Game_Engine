#include "twistedpch.h"
#include "CCamera.h"
#include "Utils/GlmUtils.h"

#include "Twisted/Game/World.h"
#include "Twisted/Game/Components/CTransform.h"

namespace Twisted
{
	Mat4x4f CCamera::GetViewMatrix()const
	{
		const CTransform& transform = m_world->GetComponent<CTransform>(m_entityID);

		auto position = transform.GetWorldPosition();
		auto target = position + transform.GetWorldForward();
		auto viewMat = glm::lookAt(position, target, Directions::Up);

		//std::cout << "World Pos:\n " << glm::to_string(position) << std::endl;
		//std::cout << "Target:\n " << glm::to_string(target) << std::endl;
		//std::cout << "View:\n " << glm::to_string(viewMat) << std::endl;

		return viewMat;
	}

	Mat4x4f CCamera::GetProjectionMatrix()
	{
		switch (CameraType)
		{
		case CameraProjectionType::PERSPECTIVE:
			return glm::perspective(GetFOVinRad(), AspectRatio, NearPlane, FarPlane);
		case CameraProjectionType::ORTOGRAPHIC:
			return glm::ortho(LeftEdge, RightEdge, BotEdge, TopEdge, NearPlane, FarPlane);
		default:
			TWISTED_ERROR("Unsupported projection type!");
			return glm::ortho(LeftEdge, RightEdge, BotEdge, TopEdge, NearPlane, FarPlane);
		}
	}
}
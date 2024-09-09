#include "pch.h"
#include "CCamera.h"

#include <gtc/matrix_transform.hpp>
#include "gtx/string_cast.hpp"


namespace Twisted
{
	Mat4x4f CCamera::GetViewMatrix(const GameCore& game)const
	{
		const CTransform& transform = game.GetComponent<CTransform>(m_entityID);

		auto position = transform.GetWorldPosition(game);
		auto target = position + transform.GetWorldForward(game);
		auto viewMat = glm::lookAt(position, target, Collections::Directions::Up);

		//std::cout << "World Pos:\n " << glm::to_string(position) << std::endl;
		//std::cout << "Target:\n " << glm::to_string(target) << std::endl;
		//std::cout << "View:\n " << glm::to_string(viewMat) << std::endl;

		return viewMat;
	}

	Mat4x4f CCamera::GetProjectionMatrix()const
	{
		if (CameraType == CameraProjectionType::ORTOGRAPHIC)
			return glm::ortho(LeftEdge, RightEdge, BotEdge, TopEdge, NearPlane, FarPlane);
		return glm::perspective(GetFOVinRad(), AspectRatio, NearPlane, FarPlane);
	}
}
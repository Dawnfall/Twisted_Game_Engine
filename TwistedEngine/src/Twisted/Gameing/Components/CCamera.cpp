#include "CCamera.h"
#include "Twisted/TwistedMacros.h"

#include "Utils/GlmUtils.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Debug/Logger.h"

namespace Twisted
{
	Mat4x4f CCamera::GetViewMatrix()const
	{
		const CTransform& transform = m_entity.GetWorld()->GetComponent<CTransform>(m_entity.GetID());

		auto position = transform.GetWorldPosition();
		auto target = position + transform.GetWorldForward();
		auto viewMat = glm::lookAt(position, target, Constants::Up);

		//std::cout << "World Pos:\n " << glm::to_string(position) << std::endl;
		//std::cout << "Target:\n " << glm::to_string(target) << std::endl;
		//std::cout << "View:\n " << glm::to_string(viewMat) << std::endl;

		return viewMat;
	}

	Mat4x4f CCamera::GetProjectionMatrix()const
	{
		switch (GetProjectionType())
		{
		case CameraProjectionType::PERSPECTIVE:
			return glm::perspective(GetFovInRad(), GetAspectRatio(), GetNearPlane(), GetFarPlane());
		case CameraProjectionType::ORTOGRAPHIC:
			return glm::ortho(GetLeftEdge(), GetRightEdge(), GetBotEdge(), GetTopEdge(), GetNearPlane(), GetFarPlane());
		default:
			TWISTED_ERROR("Unsupported projection type!");
			return glm::ortho(GetLeftEdge(), GetRightEdge(), GetBotEdge(), GetTopEdge(), GetNearPlane(), GetFarPlane());
		}
	}

	//Serialization
	void CCamera::Serialize(BinSerializer& buffer)const
	{
		buffer.Write<CameraProjectionType>(m_projectionType, nullptr);
		buffer.Write<float>(m_nearPlane, nullptr);
		buffer.Write<float>(m_farPlane, nullptr);
		buffer.Write<float>(m_fovDeg, nullptr);
		buffer.Write<float>(m_aspectRatio, nullptr);
		buffer.Write<float>(m_leftEdge, nullptr);
		buffer.Write<float>(m_rightEdge, nullptr);
		buffer.Write<float>(m_botEdge, nullptr);
		buffer.Write<float>(m_topEdge, nullptr);
	}

	void CCamera::Deserialize(BinSerializer& buffer)
	{
		m_projectionType = buffer.Read<CameraProjectionType>(nullptr);
		m_nearPlane = buffer.Read<float>(nullptr);
		m_farPlane = buffer.Read<float>(nullptr);
		m_fovDeg = buffer.Read<float>(nullptr);
		m_aspectRatio = buffer.Read<float>(nullptr);
		m_leftEdge = buffer.Read<float>(nullptr);
		m_rightEdge = buffer.Read<float>(nullptr);
		m_botEdge = buffer.Read<float>(nullptr);
		m_topEdge = buffer.Read<float>(nullptr);
	}
}

REGISTER_COMPONENT(CCamera, "CCamera");
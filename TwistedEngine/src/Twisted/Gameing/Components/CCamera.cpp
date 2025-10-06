#include "CCamera.h"
#include "Utils/GlmUtils.h"
#include "Constants.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Logger.h"

namespace Twisted
{
	Mat4x4f CCamera::GetViewMatrix()const
	{
		const CTransform& transform = GetWorld()->GetComponent<CTransform>(m_entityID);

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
	void CCamera::Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const
	{
		buffer.Write<CameraProjectionType>(GetProjectionType());
		buffer.Write<float>(GetNearPlane());
		buffer.Write<float>(GetFarPlane());
		buffer.Write<float>(GetFovInDeg());
		buffer.Write<float>(GetAspectRatio());
		buffer.Write<float>(GetLeftEdge());
		buffer.Write<float>(GetRightEdge());
		buffer.Write<float>(GetBotEdge());
		buffer.Write<float>(GetTopEdge());
	}

	void CCamera::Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)
	{
		SetProjectionType(buffer.Read<CameraProjectionType>());
		SetNearPlane(buffer.Read<float>());
		SetFarPlane(buffer.Read<float>());
		SetFovInDeg(buffer.Read<float>());
		SetAspectRatio(buffer.Read<float>());
		SetLeftEdge(buffer.Read<float>());
		SetRightEdge(buffer.Read<float>());
		SetBotEdge(buffer.Read<float>());
		SetTopEdge(buffer.Read<float>());
	}
}
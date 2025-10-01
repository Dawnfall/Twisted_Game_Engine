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

	//Serialization
	void CCamera::Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const
	{
		buffer.Write<CameraProjectionType>(CameraType);
		buffer.Write<float>(NearPlane);
		buffer.Write<float>(FarPlane);
		buffer.Write<float>(FovDeg);
		buffer.Write<float>(AspectRatio);
		buffer.Write<float>(LeftEdge);
		buffer.Write<float>(RightEdge);
		buffer.Write<float>(BotEdge);
		buffer.Write<float>(TopEdge);
	}

	void CCamera::Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)
	{
		CameraType = buffer.Read<CameraProjectionType>();
		NearPlane = buffer.Read<float>();
		FarPlane = buffer.Read<float>();
		FovDeg = buffer.Read<float>();
		AspectRatio = buffer.Read<float>();
		LeftEdge = buffer.Read<float>();
		RightEdge = buffer.Read<float>();
		BotEdge = buffer.Read<float>();
		TopEdge = buffer.Read<float>();
	}
}
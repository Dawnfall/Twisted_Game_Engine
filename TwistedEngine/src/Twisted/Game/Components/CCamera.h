#pragma once

#include "Twisted/Game/AComponent.h"
#include "Twisted/Game/Components/CTransform.h"

#include "Debug/Logger.h"

namespace Twisted
{
	enum CameraProjectionType
	{
		PERSPECTIVE,
		ORTOGRAPHIC
	};

	class CCamera :public AComponent
	{
	public:
		CCamera(entt::entity parentEntity) : AComponent(parentEntity)
		{}

		float GetFOVinRad()const
		{
			return glm::radians(FovDeg);
		}

		CameraProjectionType CameraType = CameraProjectionType::PERSPECTIVE;

		float NearPlane = 0.01f;
		float FarPlane = 100.0f;

		//	//perspetive proj
		float FovDeg = 45.0f; //in deg
		float AspectRatio = 1.0f;

		//	//ortographic proj
		float LeftEdge = 1.0f;
		float RightEdge = 1.0f;
		float BotEdge = 1.0f;
		float TopEdge = 1.0f;

		Mat4x4f GetViewMatrix(const EcsManager& ecs)const
		{
			const CTransform* transform = ecs.GetComponent<CTransform>(GetID());

			auto position = transform->GetWorldPosition(ecs);
			auto target = position + transform->GetWorldForward(ecs);
			auto viewMat = glm::lookAt(position, target, Directions::Up);

			//std::cout << "World Pos:\n " << glm::to_string(position) << std::endl;
			//std::cout << "Target:\n " << glm::to_string(target) << std::endl;
			//std::cout << "View:\n " << glm::to_string(viewMat) << std::endl;

			return viewMat;
		}


		Mat4x4f GetProjectionMatrix()
		{
			switch (CameraType)
			{
			case CameraProjectionType::PERSPECTIVE:
				return glm::perspective(GetFOVinRad(), AspectRatio, NearPlane, FarPlane);
			case CameraProjectionType::ORTOGRAPHIC:
				return glm::ortho(LeftEdge, RightEdge, BotEdge, TopEdge, NearPlane, FarPlane);
			default:
				TWISTED_ERROR("Unsupported projection type!");
				return glm::ortho(LeftEdge, RightEdge, BotEdge, TopEdge, NearPlane, FarPlane); //TODO:...
			}

		}
	};
}

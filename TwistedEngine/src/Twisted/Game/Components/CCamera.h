#pragma once

#include "Twisted/Game/AComponent.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Collections/Vector.h"

#include "Twisted/GameCore.h"

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

		CameraProjectionType CameraType=CameraProjectionType::PERSPECTIVE;

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

		Mat4x4f GetViewMatrix(const GameCore& game)const;
		Mat4x4f GetProjectionMatrix()const;
	};
}

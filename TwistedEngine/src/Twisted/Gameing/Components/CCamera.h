#pragma once

#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/RegisterLayer/Serialization/BinSerializer.h"

namespace Twisted
{
	enum TWISTED_API CameraProjectionType
	{
		PERSPECTIVE,
		ORTOGRAPHIC
	};

	class TWISTED_API CCamera :public AComponent
	{
	public:
		CCamera(EntityID entity,World* world) : AComponent(entity,world)
		{

		}

		float GetFOVinRad()const
		{
			return glm::radians(FovDeg);
		}

		Mat4x4f GetViewMatrix()const;
		Mat4x4f GetProjectionMatrix()const;

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

		void Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const override;
		void Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer) override;
	};
}

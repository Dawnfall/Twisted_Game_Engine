#pragma once

#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Game/AComponent.h"
#include "Game/Serialization/WorldSerializer.h"
#include "Game/Entity.h"

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
		CCamera(EntityID entityID, World* world) : AComponent(entityID, world)
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

		//friend void Serialize(const CCamera& camera, BinSerializer& serializer);
		//friend void Deserialize(CCamera& camera, BinSerializer& serializer);
	};
}
//namespace Twisted
//{
//	template<>
//	inline void Serialize(const CCamera& camera, BinSerializer& serializer)
//	{
//		serializer.Write<CameraProjectionType>(camera.CameraType);
//		serializer.Write<float>(camera.NearPlane);
//		serializer.Write<float>(camera.FarPlane);
//		serializer.Write<float>(camera.FovDeg);
//		serializer.Write<float>(camera.AspectRatio);
//		serializer.Write<float>(camera.LeftEdge);
//		serializer.Write<float>(camera.RightEdge);
//		serializer.Write<float>(camera.BotEdge);
//		serializer.Write<float>(camera.TopEdge);
//	}
//
//	template<>
//	inline void Deserialize(CCamera& camera, BinSerializer& serializer)
//	{
//		camera.CameraType = serializer.Read<CameraProjectionType>();
//		camera.NearPlane = serializer.Read<float>();
//		camera.FarPlane = serializer.Read<float>();
//		camera.FovDeg = serializer.Read<float>();
//		camera.AspectRatio = serializer.Read<float>();
//		camera.LeftEdge = serializer.Read<float>();
//		camera.RightEdge = serializer.Read<float>();
//		camera.BotEdge = serializer.Read<float>();
//		camera.TopEdge = serializer.Read<float>();
//	}
//}
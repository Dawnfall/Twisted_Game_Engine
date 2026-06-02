#pragma once

#include "AppCore.h"
#include "Debug/Logger.h"
#include "Utils/GlmUtils.h"
#include <magic_enum/magic_enum.hpp>
#include "AComponent.h"
#include "FrameBuffer.h"
#include "WorldRegistry.h"
#include "Utils/YamlUtils.h"
#include "Components/CTransform.h"
#include "Managers/CameraManager.h"
#include "Data/ClearParams.h"

namespace Twisted
{
	enum class CameraProjectionType :int
	{
		PERSPECTIVE,
		ORTHOGRAPHIC,
		COUNT
	};

	class TWISTED_API CameraComponent : public AComponent
	{
		TCLASS_BODY()
	public:
		CameraComponent(Entity e) : AComponent(e)
		{
			CamManager = &GetWorld()->ForceGetManager<CameraManager>();
		}

		float GetFovInRad()const { return glm::radians(FovDeg); }
		void SetFovInRad(float fovInRad) { FovDeg = glm::degrees(fovInRad); }

		bool IsMainCamera()const
		{
			return CamManager->GetMainCamera() == this;
		}

		void SetAsMainCamera()
		{
			CamManager->SetAsMainCamera(this);
		}

		Mat4x4f GetViewMatrix()const
		{
			const TransformComponent& transform = GetWorld()->GetComponent<TransformComponent>(GetID());

			auto position = transform.GetWorldPosition();
			auto target = position + transform.GetWorldForward();
			auto viewMat = glm::lookAt(position, target, Constants::Up);

			//TWISTED_INFO("World Pos: {}", glm::to_string(position));
			//TWISTED_INFO("Target: {}", glm::to_string(target));
			//TWISTED_INFO("View: {}", glm::to_string(viewMat));

			return viewMat;
		}

		Mat4x4f GetProjectionMatrix()const
		{
			switch (ProjectionType)
			{
			case CameraProjectionType::PERSPECTIVE:
				return glm::perspective(GetFovInRad(), AspectRatio, NearPlane, FarPlane);
			case CameraProjectionType::ORTHOGRAPHIC:
				return glm::ortho(LeftEdge, RightEdge, BotEdge, TopEdge, NearPlane, FarPlane);
			case CameraProjectionType::COUNT:
				TWISTED_ERROR("Unsupported projection type!");
				return glm::perspective(GetFovInRad(), AspectRatio, NearPlane, FarPlane);
			}
		}

		TPROPERTY()
		CameraProjectionType ProjectionType = CameraProjectionType::PERSPECTIVE;
		//perspetive proj
		TPROPERTY()
		float FovDeg = 45.0f; //in deg
		TPROPERTY()
		float AspectRatio = 1.0f;
		TPROPERTY()
		float NearPlane = 0.01f;
		TPROPERTY()
		float FarPlane = 100.0f;
		//ortographic proj
		TPROPERTY()
		float LeftEdge = 1.0f;
		TPROPERTY()
		float RightEdge = 1.0f;
		TPROPERTY()
		float BotEdge = 1.0f;
		TPROPERTY()
		float TopEdge = 1.0f;

		ClearParams clearParams;
		WPtr<Framebuffer> Fb = nullptr;
		CameraManager* CamManager = nullptr;
	};

	template<>
	inline void OnCreateComponent(CameraComponent& camera)
	{
		camera.Fb = WPtr<Framebuffer>(TObject::Create<Framebuffer>(camera.GetWorld()->GetName() + "_camera_fb"));

		if (!camera.CamManager->GetMainCamera())
			camera.SetAsMainCamera();
	}

	template<>
	inline void OnDestroyComponent<CameraComponent>(CameraComponent& camera)
	{
		TObject::Destroy(camera.Fb.get());
		camera.Fb = nullptr;

		if (camera.IsMainCamera())
			camera.CamManager->SetAsMainCamera(nullptr);
	}

}

template<>
struct YAML::convert<Twisted::CameraProjectionType>
{
	static YAML::Node encode(const Twisted::CameraProjectionType& rhs)
	{
		YAML::Node node;
		node = std::string(magic_enum::enum_name(rhs));
		return node;
	}

	static bool decode(const YAML::Node& node, Twisted::CameraProjectionType& rhs)
	{
		if (!node.IsScalar())
			return false;

		auto result = magic_enum::enum_cast<Twisted::CameraProjectionType>(node.as<std::string>());
		if (!result)
		{
			TWISTED_WARN("Unsupported projection type: {}", node.as<std::string>());
			rhs = Twisted::CameraProjectionType::PERSPECTIVE;
		}
		else
		{
			rhs = *result;
		}
		return true;
	}
};



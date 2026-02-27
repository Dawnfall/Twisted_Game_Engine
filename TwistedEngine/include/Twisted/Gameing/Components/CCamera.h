#pragma once

#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Gameing/AComponent.h"
#include "Serialization/BinSerializer.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Utils/YamlUtils.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Twisted/Gameing/Managers/CameraManager.h"
#include "Twisted/Rendering/Data/ClearParams.h"

namespace Twisted
{
	inline const char* PERSPECTIVE_PROJ_NAME = "perspective";
	inline const char* ORTHOGRAPHIC_PROJ_NAME = "orthographic";

	enum class TWISTED_API CameraProjectionType :int
	{
		PERSPECTIVE,
		ORTHOGRAPHIC,
		COUNT
	};

	inline static const std::string ProjTypeToString(CameraProjectionType type)
	{
		switch (type)
		{
		case CameraProjectionType::PERSPECTIVE:  return PERSPECTIVE_PROJ_NAME;
		case CameraProjectionType::ORTHOGRAPHIC: return ORTHOGRAPHIC_PROJ_NAME;
		default:
			throw std::exception("unsupported projection type");
		}
	}

	inline static CameraProjectionType ProjTypeFromString(const std::string& str)
	{
		if (str == PERSPECTIVE_PROJ_NAME) return CameraProjectionType::PERSPECTIVE;
		if (str == ORTHOGRAPHIC_PROJ_NAME) return CameraProjectionType::ORTHOGRAPHIC;
		TWISTED_WARN("Unsuported Proj type: {}", str);
		return CameraProjectionType::PERSPECTIVE;
	}

	class TWISTED_API CameraComponent :public AComponent
	{
	public:
		CameraComponent(Entity entity) : AComponent(entity)
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

			//std::cout << "World Pos:\n " << glm::to_string(position) << std::endl;
			//std::cout << "Target:\n " << glm::to_string(target) << std::endl;
			//std::cout << "View:\n " << glm::to_string(viewMat) << std::endl;

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
			default:
				TWISTED_ERROR("Unsupported projection type!");
				return glm::perspective(GetFovInRad(), AspectRatio, NearPlane, FarPlane);
			}
		}

		CameraProjectionType ProjectionType = CameraProjectionType::PERSPECTIVE;
		//perspetive proj
		float FovDeg = 45.0f; //in deg
		float AspectRatio = 1.0f;
		float NearPlane = 0.01f;
		float FarPlane = 100.0f;
		//ortographic proj
		float LeftEdge = 1.0f;
		float RightEdge = 1.0f;
		float BotEdge = 1.0f;
		float TopEdge = 1.0f;

		ClearParams clearParams;
		WPtr<Framebuffer> Fb = nullptr;
		CameraManager* CamManager = nullptr;
	};

	template<>
	inline void OnCreateComponent(CameraComponent& camera)
	{
		if (!camera.CamManager->GetMainCamera())
			camera.SetAsMainCamera();
	}

	template<>
	inline void OnDestroyComponent<CameraComponent>(CameraComponent& camera)
	{
		if (camera.IsMainCamera())
			camera.CamManager->SetAsMainCamera(nullptr);
	}

	template<>
	inline  YAML::Node YamlSerialize<CameraComponent>(const CameraComponent& camera)
	{
		YAML::Node node;

		node["proj"] = camera.ProjectionType;
		node["near"] = camera.NearPlane;
		node["far"] = camera.FarPlane;
		node["fov"] = camera.FovDeg;
		node["apect"] = camera.AspectRatio;
		node["left"] = camera.LeftEdge;
		node["right"] = camera.RightEdge;
		node["bot"] = camera.BotEdge;
		node["top"] = camera.TopEdge;

		return node;
	}
	template<>
	inline void YamlDeserialize<CameraComponent>(CameraComponent& camera, const YAML::Node& node)
	{
		camera.ProjectionType = node["proj"].as<CameraProjectionType>(CameraProjectionType::PERSPECTIVE);
		camera.NearPlane = node["near"].as<float>(1.0f);
		camera.FarPlane = node["far"].as<float>(1.0f);
		camera.FovDeg = node["fov"].as<float>(45.0f);
		camera.AspectRatio = node["apect"].as<float>(1.333f);
		camera.LeftEdge = node["left"].as<float>(1.0f);
		camera.RightEdge = node["right"].as<float>(1.0f);
		camera.BotEdge = node["bot"].as<float>(1.0f);
		camera.TopEdge = node["top"].as<float>(1.0f);
	}

	template<typename T>
	inline void BinSerialize(const CameraComponent& camera, BinSerializer& buffer)
	{
		buffer.Write<CameraProjectionType>(camera.ProjectionType, nullptr);
		buffer.Write<float>(camera.NearPlane, nullptr);
		buffer.Write<float>(camera.FarPlane, nullptr);
		buffer.Write<float>(camera.FovDeg, nullptr);
		buffer.Write<float>(camera.AspectRatio, nullptr);
		buffer.Write<float>(camera.LeftEdge, nullptr);
		buffer.Write<float>(camera.RightEdge, nullptr);
		buffer.Write<float>(camera.BotEdge, nullptr);
		buffer.Write<float>(camera.TopEdge, nullptr);
	}

	template<typename T>
	inline void BinDeserialize(CameraComponent& camera, BinSerializer& buffer)
	{
		camera.ProjectionType = buffer.Read<CameraProjectionType>(nullptr);
		camera.NearPlane = buffer.Read<float>(nullptr);
		camera.FarPlane = buffer.Read<float>(nullptr);
		camera.FovDeg = buffer.Read<float>(nullptr);
		camera.AspectRatio = buffer.Read<float>(nullptr);
		camera.LeftEdge = buffer.Read<float>(nullptr);
		camera.RightEdge = buffer.Read<float>(nullptr);
		camera.BotEdge = buffer.Read<float>(nullptr);
		camera.TopEdge = buffer.Read<float>(nullptr);
	}
}

template<>
struct YAML::convert<Twisted::CameraProjectionType>
{
	static YAML::Node encode(const Twisted::CameraProjectionType& rhs)
	{
		YAML::Node node;
		node = Twisted::ProjTypeToString(rhs);
		return node;
	}

	static bool decode(const YAML::Node& node, Twisted::CameraProjectionType& rhs)
	{
		if (!node.IsScalar())
			return false;

		const std::string value = node.as<std::string>();
		rhs = Twisted::ProjTypeFromString(value);

		return true;
	}
};

REGISTER_COMPONENT(CameraComponent, "CCamera");


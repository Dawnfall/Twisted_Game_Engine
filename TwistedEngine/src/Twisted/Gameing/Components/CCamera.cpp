#include "CCamera.h"

#include "Utils/GlmUtils.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Components/CTransform.h"
#include "Debug/Logger.h"
#include "CMainCamera.h"

namespace Twisted
{
	CCamera::~CCamera()
	{
		GetWorld()->RemoveComponent<CMainCamera>(GetID());
	}
	bool CCamera::IsMainCamera()const
	{
		return m_entity.GetWorld()->HasComponent<CMainCamera>(GetID());
	}

	void CCamera::SetAsMainCamera()
	{
		World* world = m_entity.GetWorld();

		CMainCamera* mainCamera = world->FindFirstOfType<CMainCamera>();
		if (mainCamera)
		{
			if (mainCamera->GetEntity() == m_entity)
				return;
			mainCamera->GetEntity().GetWorld()->RemoveComponent<CMainCamera>(mainCamera->GetID());
		}
		GetWorld()->AddComponent<CMainCamera>(GetID());
	}

	Mat4x4f CCamera::GetViewMatrix()const
	{
		const CTransform& transform = GetWorld()->GetComponent<CTransform>(GetID());

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
		case CameraProjectionType::ORTHOGRAPHIC:
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

	YAML::Node CCamera::YamlSerialize() const
	{
		YAML::Node node;

		node["proj"] = m_projectionType;
		node["near"] = m_nearPlane;
		node["far"] = m_farPlane;
		node["fov"] = m_fovDeg;
		node["apect"] = m_aspectRatio;
		node["left"] = m_leftEdge;
		node["right"] = m_rightEdge;
		node["bot"] = m_botEdge;
		node["top"] = m_topEdge;

		return node;
	}
	void CCamera::YamlDeserialize(const YAML::Node& node)
	{
		m_projectionType = node["proj"].as<CameraProjectionType>(CameraProjectionType::PERSPECTIVE);
		m_nearPlane= node["near"].as<float>(1.0f);
		m_farPlane= node["far"].as<float>(1.0f);
		m_fovDeg = node["fov"].as<float>(45.0f);
		m_aspectRatio = node["apect"].as<float>(1.333f);
		m_leftEdge = node["left"].as<float>(1.0f);
		m_rightEdge = node["right"].as<float>(1.0f);
		m_botEdge = node["bot"].as<float>(1.0f);
		m_topEdge = node["top"].as<float>(1.0f);
	}
}


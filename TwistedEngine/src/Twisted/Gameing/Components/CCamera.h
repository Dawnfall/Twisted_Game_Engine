#pragma once

#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Gameing/AComponent.h"
#include "Serialization/BinSerializer.h"
#include "Twisted/Rendering/FrameBuffer.h"
#include "CMainCamera.h"
#include "Twisted/Gameing/WorldRegistry.h"

namespace Twisted
{
	enum class TWISTED_API CameraProjectionType :int
	{
		PERSPECTIVE,
		ORTHOGRAPHIC,
		COUNT
	};

	inline const char* ProjTypeToString(CameraProjectionType type)
	{
		switch (type)
		{
		case CameraProjectionType::PERSPECTIVE:  return "Perspective";
		case CameraProjectionType::ORTHOGRAPHIC: return "Orthographic";
		default:           throw std::exception("unsupported projection type");
		}
	}

	inline CameraProjectionType ProjTypeFromString(const std::string& str)
	{
		if (str == "Perspective") return CameraProjectionType::PERSPECTIVE;
		if (str == "Orthographic") return CameraProjectionType::ORTHOGRAPHIC;
		throw std::exception("unsupported projection type string");
	}

	class TWISTED_API CCamera :public AComponent
	{
	public:
		CCamera(Entity entity) : AComponent(entity)
		{
		}
		~CCamera();
		bool IsMainCamera()const;
		void SetAsMainCamera();

		Mat4x4f GetViewMatrix()const;
		Mat4x4f GetProjectionMatrix()const;

		CameraProjectionType GetProjectionType()const { return m_projectionType; }
		void SetProjectionType(CameraProjectionType projType) { m_projectionType = projType; }

		float GetNearPlane()const { return m_nearPlane; }
		float GetFarPlane()const { return m_farPlane; }
		void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
		void SetFarPlane(float farPlane) { m_farPlane = farPlane; }

		float GetFovInRad()const { return glm::radians(m_fovDeg); }
		float GetFovInDeg() const { return m_fovDeg; }
		void SetFovInRad(float fovInRad) { m_fovDeg = glm::degrees(fovInRad); }
		void SetFovInDeg(float fovInDeg) { m_fovDeg = fovInDeg; }

		float GetAspectRatio() const { return m_aspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }

		float GetLeftEdge() const { return m_leftEdge; }
		float GetRightEdge() const { return m_rightEdge; }
		float GetBotEdge() const { return m_botEdge; }
		float GetTopEdge() const { return m_topEdge; }
		void SetLeftEdge(float leftEdge) { m_leftEdge = leftEdge; }
		void SetRightEdge(float rightEdge) { m_rightEdge = rightEdge; }
		void SetBotEdge(float botEdge) { m_botEdge = botEdge; }
		void SetTopEdge(float topEdge) { m_topEdge = topEdge; }

		void Serialize(BinSerializer& buffer)const override;
		void Deserialize(BinSerializer& buffer) override;

		YAML::Node YamlSerialize() const override;
		void YamlDeserialize(const YAML::Node& node) override;

		void SetFrameBuffer(FrameBuffer* buffer) { m_frameBuffer = buffer; }
		FrameBuffer* GetFrameBuffer() { return m_frameBuffer.get(); }

	private:

		CameraProjectionType m_projectionType = CameraProjectionType::PERSPECTIVE;

		//perspetive proj
		float m_fovDeg = 45.0f; //in deg
		float m_aspectRatio = 1.0f;
		float m_nearPlane = 0.01f;
		float m_farPlane = 100.0f;

		//ortographic proj
		float m_leftEdge = 1.0f;
		float m_rightEdge = 1.0f;
		float m_botEdge = 1.0f;
		float m_topEdge = 1.0f;

		WPtr<FrameBuffer> m_frameBuffer = nullptr;
	};
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

REGISTER_COMPONENT(CCamera, "CCamera");
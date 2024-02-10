#pragma once

#include "Dawn/AComponent.h"
#include "Dawn/Game/Components/Transform.h"

#include <gtc/matrix_transform.hpp>
#include "Dawn/Game/World.h"

namespace Dawn
{
	enum CameraProjectionType
	{
		PERSPECTIVE,
		ORTOGRAPHIC
	};

	class CCamera :public AComponent
	{
	public:
		CCamera(entt::entity parentEntity, World* world) : AComponent(parentEntity, world)
		{
		}

		void SetCameraProjectionType(CameraProjectionType cameraProjectionType)
		{
			m_cameraType = cameraProjectionType;
		}
		CameraProjectionType GetCAmeraProjectionType()const
		{
			return m_cameraType;
		}

		float GetFOVinDeg()const
		{
			return m_fov;
		}
		float GetFOVinRad()const
		{
			return glm::radians(m_fov);
		}

		float GetNearPlane()const
		{
			return m_nearPlane;
		}
		float GetFarPlane()const
		{
			return m_farPlane;
		}

		float GetAspectRation()const
		{
			return m_aspectRatio;
		}

		float GetLeftEdge()const
		{
			return m_leftEdge;
		}
		float GetRightEdge()const
		{
			return m_rightEdge;
		}
		float GetBotEdge()const
		{
			return m_botEdge;
		}
		float GetTopEdge()const
		{
			return m_topEdge;
		}

		void SetLeftEdge(float leftEdge)
		{
			m_leftEdge = leftEdge;
		}
		void SetRightEdge(float rightEdge)
		{
			m_rightEdge = rightEdge;
		}
		void SetBotEdge(float botEdge)
		{
			m_botEdge = botEdge;
		}
		void SetTopEdge(float topEdge)
		{
			m_topEdge = topEdge;
		}

		float SetNearPlane(float nearPlane)
		{
			m_nearPlane = nearPlane;
		}
		float SetFarPlane(float farPlane)
		{
			m_farPlane = farPlane;
		}

		void SetAspectRatio(float newAspectRatio)
		{
			m_aspectRatio = newAspectRatio;
		}

		void SetFOV(float newFOVinDeg)
		{
			m_fov = newFOVinDeg;
		}

		glm::mat4 GetViewMatrix()const;
		glm::mat4 GetProjectionMatrix()const;

	private:
		CameraProjectionType m_cameraType;

		float m_nearPlane = 0.01f;;
		float m_farPlane = 100.0f;

		//perspetive proj
		float m_fov = 45.0f; //in deg
		float m_aspectRatio;

		//ortographic proj
		float m_leftEdge;
		float m_rightEdge;
		float m_botEdge;
		float m_topEdge;
	};
}

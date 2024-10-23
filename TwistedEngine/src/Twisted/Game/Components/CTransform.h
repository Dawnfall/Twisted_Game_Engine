#pragma once

#include "pch.h"

#include "Twisted/Game/EcsManager.h"
#include "Collections/Geometry.h"
#include "Twisted/Game/AComponent.h" 
#include "Utils/Utils.h"
#include "Twisted/Game/Transformations.h"


namespace Twisted
{
	enum class RelativeSpace
	{
		LOCAL,
		WORLD
	};

	class CTransform :public AComponent
	{
	public:
#pragma region Constructors
		CTransform(entt::entity entity);
		CTransform(entt::entity entity, const Vec3f& position, const Quat& rotation, const Vec3f& scale);
#pragma endregion

		//*******************
		// sets / gets 

		const Vec3f& GetLocalPosition() const { return m_position; }
		const Quat& GetLocalRotation() const { return m_rotation; }
		const glm::vec3& GetLocalScale() const { return m_scale; }
		const Vec3f GetLocalRotationEulerRad() const { return glm::eulerAngles(m_rotation); }

		void SetLocalPosition(const Vec3f& newPositon) { m_position = newPositon; }
		void SetLocalRotation(const Quat& newRotation) { m_rotation = newRotation; }
		void SetLocalScale(const Vec3f& newScale) { m_scale = newScale; }
		void SetWorldPosition(const Vec3f& newPosition, const EcsManager& ecs) { m_position = WorldToLocalPoint(newPosition, ecs); }

		const Vec3f GetWorldPosition(const EcsManager& ecs) const
		{
			return LocalToWorldPoint(Directions::Zero, ecs);
		}
		const Quat GetWorldRotation(const EcsManager& ecs) const //TODO... not sure is correct
		{
			if (m_parentID != entt::null)
				return ecs.GetComponent<CTransform>(m_parentID)->GetWorldRotation(ecs) * GetLocalRotation();
			return GetLocalRotation();
		}
		const Vec3f GetWorldScale(const EcsManager& ecs) const
		{
			if (m_parentID != entt::null)
				return ecs.GetComponent<CTransform>(m_parentID)->GetWorldScale(ecs) * GetLocalScale(); //not sure correct
			return GetLocalScale();
		}
		const Vec3f GetWorldRotationEuler(const EcsManager& ecs) const
		{
			return glm::eulerAngles(GetWorldRotation(ecs));
		}

		//**************
		// Transformation matrices

		Mat4x4f GetLocalTranslationMatrix()const { return glm::translate(glm::mat4(1.0f), GetLocalPosition()); }
		Mat4x4f GetLocalRotationMatrix()const { return glm::toMat4(GetLocalRotation()); }
		Mat4x4f GetLocalScaleMatrix()const { return glm::scale(glm::mat4(1.0f), GetLocalScale()); }
		Mat4x4f GetLocalModelMatrix()const { return	GetLocalTranslationMatrix() * GetLocalRotationMatrix() * GetLocalScaleMatrix(); }

		Mat4x4f GetInvertTranslationMatrix()const { return glm::translate(glm::mat4(1.0f), -GetLocalPosition()); }
		Mat4x4f GetInvertRotationMatrix() const { return glm::toMat4(glm::conjugate(GetLocalRotation())); }
		Mat4x4f GetInvertScaleMatrix() const { return glm::scale(glm::mat4(1.0f), 1.0f / GetLocalScale()); }
		Mat4x4f GetInvertLocalModelMatrix()const { return GetInvertScaleMatrix() * GetInvertRotationMatrix() * GetInvertTranslationMatrix(); }

		Mat4x4f GetWorldModelMatrix(const EcsManager& ecs)const
		{
			if (GetParentID() != entt::null)
			{
				CTransform parentTransform = *ecs.GetComponent<CTransform>(GetParentID());
				return  parentTransform.GetWorldModelMatrix(ecs) * GetLocalModelMatrix();
			}
			return GetLocalModelMatrix();
		}
		Mat4x4f GetWorldInvertModelMatrix(const EcsManager& ecs)const
		{
			if (GetParentID() != entt::null)
			{
				CTransform parentTransform = *ecs.GetComponent<CTransform>(GetParentID());
				return GetInvertLocalModelMatrix() * parentTransform.GetWorldInvertModelMatrix(ecs);
			}
			return GetInvertLocalModelMatrix();
		}

		//***************
		// Local <--> world

		Vec3f LocalToWorldVector(const Vec3f& vec, const EcsManager& ecs)const
		{
			glm::vec4 worldVec = GetWorldModelMatrix(ecs) * glm::vec4(vec, 0.0f);
			return glm::vec3(worldVec.x, worldVec.y, worldVec.z);
		}
		Vec3f LocalToWorldPoint(const Vec3f& point, const EcsManager& ecs)const
		{
			auto worldMat = GetWorldModelMatrix(ecs);
			auto pVec4 = glm::vec4(point, 1.0f);

			glm::vec4 worldPoint = GetWorldModelMatrix(ecs) * glm::vec4(point, 1.0f);
			return glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
		}
		Vec3f WorldToLocalVector(const Vec3f& vec, const EcsManager& ecs)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix(ecs) * glm::vec4(vec, 0.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}
		Vec3f WorldToLocalPoint(const Vec3f& vec, const EcsManager& ecs)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix(ecs) * glm::vec4(vec, 1.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}

		Vec3f GetWorldForward(const EcsManager& ecs)const
		{
			return LocalToWorldVector(Directions::Forward, ecs);
		}
		Vec3f GetWorldRight(const EcsManager& ecs)const
		{
			return LocalToWorldVector(Directions::Right, ecs);
		}
		Vec3f GetWorldUp(const EcsManager& ecs)const
		{
			return LocalToWorldVector(Directions::Up, ecs);
		}

		//****************
		// Transformations

		void Translate(const Vec3f& translation)
		{
			m_position += translation;
		}
		void Rotate(const Quat& rotation)
		{
			m_rotation = glm::normalize(m_rotation * rotation);
		}
		void Rotate(const Vec3f& eulerAnglesInDeg)
		{
			Quat rotationQuat = Quat(glm::radians(eulerAnglesInDeg));
			Rotate(rotationQuat);
		}
		void Scale(const Vec3f& scale)
		{
			m_scale *= scale;
		}
		void LookAt(const Vec3f& lookAtPoint, const Vec3f& upVector, const EcsManager& ecs)
		{
			auto lookAtMat = glm::lookAt(GetLocalPosition(), lookAtPoint, upVector);
			auto newRotation = glm::quat_cast(lookAtMat);
			SetLocalRotation(newRotation);
		}

		//***************
		// Tree

		const entt::entity& GetParentID()const { return m_parentID; }
		const std::vector<entt::entity>& GetChildIDs()const { return m_children; }
		const size_t GetChildCount() const { return m_children.size(); }

		void SetParent(entt::entity newParentID, EcsManager& ecs)
		{
			if (newParentID == m_parentID)
				return;

			CTransform* newParentTransform = ecs.GetComponent<CTransform>(newParentID);

			if (m_parentID != entt::null)
			{
				CTransform* currentParent = ecs.GetComponent<CTransform>(m_parentID);
				Utils::removeValue(currentParent->m_children, m_entityID);
				m_parentID = entt::null;
			}

			m_parentID = newParentID;
			if (newParentTransform != nullptr)
				newParentTransform->m_children.push_back(m_entityID);
		}

		//***************
		// Name
		const std::string& GetName()const { return m_name; }
		void SetName(const std::string& newName) { m_name = newName; }

	private:
		entt::entity m_parentID;
		std::vector<entt::entity> m_children;

		std::string m_name;
		Vec3f m_position;
		Quat m_rotation;
		Vec3f m_scale;
	};
}
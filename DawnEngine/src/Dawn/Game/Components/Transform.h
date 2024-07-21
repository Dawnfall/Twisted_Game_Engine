#pragma once

#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_inverse.hpp>
#include <vector>

#include "Dawn/Game/AComponent.h" 
#include "Collections/Directions.h"
#include "EnTT/entt.hpp"
#include "Utils/Utils.h"



namespace Dawn
{
	//class Transform :AComponent
	//{
//	public:
//#pragma region Constructors
//		Transform(World* world, entt::entity entity);
//		Transform(World* world, entt::entity entity, const glm::vec3 position, const glm::quat rotation, const glm::vec3 scale);
//#pragma endregion
//
//		unsigned int GetChildCount()
//		{
//			return m_children.size();
//		}
//
//		const glm::vec3& GetLocalPosition() const
//		{
//			return m_position;
//		}
//		const glm::quat& GetLocalRotation() const
//		{
//			return m_rotation;
//		}
//		glm::vec3 GetLocalEulerAngles() const
//		{
//			return glm::eulerAngles(m_rotation);
//		}
//		const glm::vec3& GetLocalScale() const
//		{
//			return m_scale;
//		}
//
//		glm::mat4 GetLocalModelMatrix() const
//		{
//			return
//				glm::translate(glm::mat4(1.0f), m_position) *
//				glm::toMat4(m_rotation) *
//				glm::scale(glm::mat4(1.0f), m_scale);
//		}
//		glm::mat4 GetWorldModelMatrix() const
//		{
//			if (m_parentID != entt::null)
//			{
//				Transform parentTransform = m_world->GetComponent<Transform>(m_parentID);
//				return parentTransform.GetWorldModelMatrix() * GetLocalModelMatrix();
//			}
//			return GetLocalModelMatrix();
//		}
//
//		glm::mat4 GetLocalInvertModelMatrix() const
//		{
//			return
//				glm::scale(glm::mat4(1.0f), 1.0f / m_scale) *
//				glm::toMat4(glm::conjugate(m_rotation)) *
//				glm::translate(glm::mat4(1.0f), -m_position);
//		}
//		glm::mat4 GetWorldInvertModelMatrix() const
//		{
//			if (m_parentID != entt::null)
//			{
//				Transform parentTransform = m_world->GetComponent<Transform>(m_parentID);
//				return GetLocalInvertModelMatrix() * parentTransform.GetWorldInvertModelMatrix();
//			}
//			return GetLocalInvertModelMatrix();
//		}
//
//		glm::vec3 LocalToWorldVector(const glm::vec3& vec)const
//		{
//			glm::vec4 worldVec = GetWorldModelMatrix() * glm::vec4(vec, 0.0f);
//			return glm::vec3(worldVec.x, worldVec.y, worldVec.z);
//		}
//		glm::vec3 LocalToWorldPoint(const glm::vec3& point)const
//		{
//			glm::vec4 worldPoint = GetWorldModelMatrix() * glm::vec4(point, 0.0f);
//			return glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
//
//		}
//
//		glm::vec3 WorldToLocalVector(const glm::vec3& vec)const
//		{
//			glm::vec4 localVec = GetWorldInvertModelMatrix() * glm::vec4(vec, 0.0f);
//			return glm::vec3(localVec.x, localVec.y, localVec.z);
//		}
//		glm::vec3 WorldToLocalPoint(const glm::vec3& vec)const
//		{
//			glm::vec4 localVec = GetWorldInvertModelMatrix() * glm::vec4(vec, 1.0f);
//			return glm::vec3(localVec.x, localVec.y, localVec.z);
//		}
//
//		glm::vec3 GetWorldPosition() const
//		{
//			return LocalToWorldPoint(m_position);
//		}
//
//		glm::vec3 GetWorldForward() const
//		{
//			return LocalToWorldVector(Collections::Directions::Forward);
//		}
//		glm::vec3 GetWorldRight()const
//		{
//			return LocalToWorldVector(Collections::Directions::Right);
//		}
//		glm::vec3 GetWorldUp()const
//		{
//			return LocalToWorldVector(Collections::Directions::Up);
//		}
//
//		// Translate the position of the transform
//		void Translate(const glm::vec3& translation)
//		{
//			m_position += translation;
//
//		}
//		// Rotate the transform using a quaternion 
//		void Rotate(const glm::quat& rotation)
//		{
//			m_rotation = m_rotation * rotation;
//		}
//		// Rotate the transform by Euler angles
//		void Rotate(const glm::vec3& eulerAngles)
//		{
//			glm::quat rotationQuat = glm::quat(eulerAngles);
//			Rotate(rotationQuat);
//		}
//
//
//		// Scale the transform
//		void Scale(const glm::vec3& scale)
//		{
//			m_scale *= scale;
//		}
//		 
//		void SetParent(entt::entity newParentID)
//		{
//			if (newParentID == m_parentID)
//				return;
//
//			Transform newParentTransform = m_world->GetComponent<Transform>(newParentID);
//	
//			if (m_parentID != entt::null)
//			{
//				Transform currentParent = m_world->GetComponent<Transform>(m_parentID);
//				Utils::removeValue(currentParent.m_children, m_entityID);
//				m_parentID = entt::null;
//			}
//
//			newParentTransform.m_children.push_back(m_entityID);
//			m_parentID = newParentID;
//		}
//
//		//void SetLocalPosition(const glm::vec3 position)
//		//{
//		//	if (!m_parent)
//		//		m_position = position;
//		//	else
//		//	{
//
//		//	}
//		//}
//
//		//glm::quat GetWorldRotation() const
//		//{
//		//	if (m_parent)
//		//		return m_parent->GetWorldRotation() * GetLocalRotation();
//		//	return m_rotation;
//		//}
//		//glm::vec3 GetWorldEulerAngles() const
//		//{
//		//	return glm::eulerAngles(GetWorldRotation());
//		//}
//
//		//const glm::vec3& GetWorldScale() const
//		//{
//		//	if (m_parent)
//		//		return m_parent->GetWorldScale() * m_scale;
//		//	return m_scale;
//		//}
//
//	private:
//
//		entt::entity m_parentID;
//		std::vector<entt::entity> m_children;
//
//		glm::vec3 m_position;
//		glm::quat m_rotation;
//		glm::vec3 m_scale;
	//};
}
#pragma once

#include "Twisted/GameCore.h"
#include "Collections/Vector.h"
#include "Twisted/Game/AComponent.h" 
#include "Collections/Directions.h"
#include "EnTT/entt.hpp"
#include "Utils/Utils.h"

#include <vector>
#include <gtc/matrix_inverse.hpp>
#include <gtx/quaternion.hpp>

#include "gtx/string_cast.hpp"
#include <iostream>

namespace Twisted
{
	enum class RelativeSpace
	{
		LOCAL,
		WORLD
	};

	struct CTransform :AComponent
	{
	public:
#pragma region Constructors
		CTransform(entt::entity entity);
		CTransform(entt::entity entity, const Vec3f& position, const Quat& rotation, const Vec3f& scale);
#pragma endregion

		const Vec3f& GetLocalPosition() const { return m_position; }
		const Quat& GetLocalRotation() const { return m_rotation; }
		const glm::vec3& GetLocalScale() const { return m_scale; }
		const Vec3f GetLocalRotationEulerRad() const { return glm::eulerAngles(m_rotation); }

		void SetLocalPosition(const Vec3f& newPositon) { m_position = newPositon; }
		void SetLocalRotation(const Quat& newRotation) { m_rotation = newRotation; }
		void SetLocalScale(const Vec3f& newScale) { m_scale = newScale; }
		void SetWorldPosition(const Vec3f& newPosition, const GameCore& game) { m_position = WorldToLocalPoint(newPosition, game); }

		//**************
		// Transformation matrices

		const Mat4x4f GetTranslationMatrix()const { return glm::translate(glm::mat4(1.0f), m_position); }
		const Mat4x4f GetRotationMatrix()const { return glm::toMat4(m_rotation); }
		const Mat4x4f GetScaleMatrix()const { return glm::scale(glm::mat4(1.0f), m_scale); }
		const Mat4x4f GetLocalModelMatrix() const
		{
			auto trans = GetTranslationMatrix();
			auto rot = GetRotationMatrix();
			auto scale = GetScaleMatrix();

			//std::cout << "Trans:\n " << glm::to_string(trans) << std::endl;
			//std::cout << "Rot:\n " << glm::to_string(rot) << std::endl;
			//std::cout << "Scale:\n " << glm::to_string(scale) << std::endl;
			//std::cout << "Model:\n " << glm::to_string(trans * rot * scale) << std::endl;

			return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
		}

		const Mat4x4f GetInvertTranslationMatrix() const { return glm::translate(glm::mat4(1.0f), -m_position); }
		const Mat4x4f GetInvertRotationMatrix()const { return glm::toMat4(glm::conjugate(m_rotation)); }
		const Mat4x4f GetInvertScaleMatrix()const { return glm::scale(glm::mat4(1.0f), 1.0f / m_scale); }
		const Mat4x4f GetInvertLocalModelMatrix() const
		{
			return	GetInvertScaleMatrix() * GetInvertRotationMatrix() * GetInvertTranslationMatrix();
		}

		const Mat4x4f GetWorldModelMatrix(const GameCore& game) const
		{
			auto localMat = GetLocalModelMatrix();

			if (m_parentID != entt::null)
			{
				auto parentWorldMat = game.GetComponent<CTransform>(m_parentID).GetWorldModelMatrix(game);
				return  parentWorldMat * localMat;
			}
			return localMat;
		}
		const Mat4x4f GetWorldInvertModelMatrix(const GameCore& game) const
		{
			if (m_parentID != entt::null)
				return GetInvertLocalModelMatrix()* game.GetComponent<CTransform>(m_parentID).GetWorldInvertModelMatrix(game);
			return GetInvertLocalModelMatrix();
		}

		const Vec3f GetWorldPosition(const GameCore& game) const
		{
			return LocalToWorldPoint(Collections::Directions::Zero, game);
		}
		const Quat GetWorldRotation(const GameCore& game) const //TODO... not sure is correct
		{
			if (m_parentID != entt::null)
				return game.GetComponent<CTransform>(m_parentID).GetWorldRotation(game) * GetLocalRotation();
			return GetLocalRotation();
		}
		const Vec3f GetWorldScale(const GameCore& game) const
		{
			if (m_parentID != entt::null)
				return game.GetComponent<CTransform>(m_parentID).GetWorldScale(game) * GetLocalScale(); //not sure correct
			return GetLocalScale();
		}
		const Vec3f GetWorldRotationEuler(const GameCore& game) const
		{
			return glm::eulerAngles(GetWorldRotation(game));
		}

		const Vec3f GetWorldForward(const GameCore& game) const
		{
			return LocalToWorldVector(Collections::Directions::Forward, game);
		}
		const Vec3f GetWorldRight(const GameCore& game)const
		{
			return LocalToWorldVector(Collections::Directions::Right, game);
		}
		const Vec3f GetWorldUp(const GameCore& game)const
		{
			return LocalToWorldVector(Collections::Directions::Up, game);
		}

		const Vec3f LocalToWorldVector(const Vec3f& vec, const GameCore& game)const
		{
			glm::vec4 worldVec = GetWorldModelMatrix(game) * glm::vec4(vec, 0.0f);
			return glm::vec3(worldVec.x, worldVec.y, worldVec.z);
		}
		const Vec3f LocalToWorldPoint(const Vec3f& point, const GameCore& game)const
		{
			auto worldMat = GetWorldModelMatrix(game);
			auto pVec4 = glm::vec4(point, 1.0f);

			glm::vec4 worldPoint = GetWorldModelMatrix(game) * glm::vec4(point, 1.0f);
			return glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
		}
		const Vec3f WorldToLocalVector(const Vec3f& vec, const GameCore& game)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix(game) * glm::vec4(vec, 0.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}
		const Vec3f WorldToLocalPoint(const Vec3f& vec, const GameCore& game)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix(game) * glm::vec4(vec, 1.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}

		void Translate(const Vec3f& translation)
		{
			m_position += translation;
		}
		void Rotate(const Quat& rotation)
		{
			m_rotation = m_rotation * rotation;
		}
		void Rotate(const Vec3f& eulerAngles)
		{
			Quat rotationQuat = Quat(eulerAngles);
			Rotate(rotationQuat);
		}
		void Scale(const Vec3f& scale)
		{
			m_scale *= scale;
		}
		void LookAt(const Vec3f& targetWorldSpace, const GameCore& game)
		{
			Vec3f targetForward = glm::normalize(WorldToLocalPoint(targetWorldSpace,game) - GetLocalPosition());
			Quat rot = glm::rotation(Collections::Directions::Forward, targetForward);

			Rotate(rot);
		}

		void SetParent(entt::entity newParentID, const GameCore& game)
		{
			if (newParentID == m_parentID)
				return;

			CTransform newParentTransform = game.GetComponent<CTransform>(newParentID);

			if (m_parentID != entt::null)
			{
				CTransform currentParent = game.GetComponent<CTransform>(m_parentID);
				Utils::removeValue(currentParent.m_children, m_entityID);
				m_parentID = entt::null;
			}

			newParentTransform.m_children.push_back(m_entityID);
			m_parentID = newParentID;
		}

		const size_t GetChildCount() const { return m_children.size(); }

	private:
		entt::entity m_parentID;
		std::vector<entt::entity> m_children;

		Vec3f m_position;
		Quat m_rotation;
		Vec3f m_scale;
	};
}
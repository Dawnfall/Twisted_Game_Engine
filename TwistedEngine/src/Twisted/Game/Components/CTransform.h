#pragma once

#include "AppCore.h"
#include "Utils/Utils.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Game/Entity.h"

#include "Twisted/Game/AComponent.h"
#include "Serialization/WorldSerializer.h"
namespace Twisted
{
	enum class TWISTED_API RelativeSpace
	{
		LOCAL,
		WORLD
	};

	class World;
	class TWISTED_API CTransform :public AComponent
	{
	public:
		CTransform(EntityID entityID, World* world);
		//	CTransform(EntityID entity, const Vec3f& position, const Quat& rotation, const Vec3f& scale);

		//***************
		// ID

		const std::string& GetName()const { return m_name; }
		void SetName(const std::string& newName) { m_name = newName; }

		//***************
		// Transform

		const Vec3f& GetLocalPosition() const { return m_position; }
		const Quat& GetLocalRotation() const { return m_rotation; }
		const glm::vec3& GetLocalScale() const { return m_scale; }
		const Vec3f GetLocalRotationEulerRad() const { return glm::eulerAngles(m_rotation); }

		void SetLocalPosition(const Vec3f& newPositon) { m_position = newPositon; }
		void SetLocalRotation(const Quat& newRotation) { m_rotation = newRotation; }
		void SetLocalScale(const Vec3f& newScale) { m_scale = newScale; }
		void SetWorldPosition(const Vec3f& newPosition) { m_position = WorldToLocalPoint(newPosition); }

		const Vec3f GetWorldPosition() const
		{
			return LocalToWorldPoint(Directions::Zero);
		}
		const Quat GetWorldRotation() const; //TODO... not sure is correct	
		const Vec3f GetWorldScale() const;
		const Vec3f GetWorldRotationEuler() const
		{
			return glm::eulerAngles(GetWorldRotation());
		}

		//***************
		// Tree

		const EntityID GetParentID()const { return m_parentID; }
		const std::vector<EntityID>& GetChildren()const { return m_childrenIDs; }
		const size_t GetChildCount() const { return m_childrenIDs.size(); }

		void SetParent(EntityID newParentID);

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

		Mat4x4f GetWorldModelMatrix()const;

		Mat4x4f GetWorldInvertModelMatrix()const;

		//***************
		// Local <--> world

		Vec3f LocalToWorldVector(const Vec3f& vec)const
		{
			glm::vec4 worldVec = GetWorldModelMatrix() * glm::vec4(vec, 0.0f);
			return glm::vec3(worldVec.x, worldVec.y, worldVec.z);
		}
		Vec3f LocalToWorldPoint(const Vec3f& point)const
		{
			auto worldMat = GetWorldModelMatrix();
			auto pVec4 = glm::vec4(point, 1.0f);

			glm::vec4 worldPoint = GetWorldModelMatrix() * glm::vec4(point, 1.0f);
			return glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
		}
		Vec3f WorldToLocalVector(const Vec3f& vec)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix() * glm::vec4(vec, 0.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}
		Vec3f WorldToLocalPoint(const Vec3f& vec)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix() * glm::vec4(vec, 1.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}

		Vec3f GetWorldForward()const
		{
			return LocalToWorldVector(Directions::Forward);
		}
		Vec3f GetWorldRight()const
		{
			return LocalToWorldVector(Directions::Right);
		}
		Vec3f GetWorldUp()const
		{
			return LocalToWorldVector(Directions::Up);
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
		void LookAt(const Vec3f& lookAtPoint, const Vec3f& upVector)
		{
			auto lookAtMat = glm::lookAt(GetLocalPosition(), lookAtPoint, upVector);
			auto newRotation = glm::quat_cast(lookAtMat);
			SetLocalRotation(newRotation);
		}

	private:

		std::string m_name;
		EntityID m_parentID;

		Vec3f m_position;
		Quat m_rotation;
		Vec3f m_scale;
		std::vector<EntityID> m_childrenIDs;

		static std::vector<CTransform*> m_rootTransforms;

	public:
		static const std::vector<CTransform*>& GetRootTransforms() { return m_rootTransforms; }

		friend void Serialize<CTransform>(const CTransform& transform, BinSerializer& serializer);
		friend void Deserialize<CTransform>(CTransform& transform, BinSerializer& serializer);
	};


	template<>
	inline void Serialize<CTransform>(const CTransform& transform, BinSerializer& serializer)
	{
		serializer.Write<std::string>(transform.m_name);
		serializer.Write<EntityID>(transform.m_parentID);
		serializer.WriteVec<EntityID>(transform.m_childrenIDs);

		serializer.Write<Vec3f>(transform.m_position);
		serializer.Write<Vec3f>(transform.m_scale);
		serializer.Write<Quat>(transform.m_rotation);
	}

	template<>
	inline void Deserialize<CTransform>(CTransform& transform, BinSerializer& serializer)
	{
		transform.m_name = serializer.Read<std::string>();
		transform.m_parentID = serializer.Read<EntityID>();
		transform.m_childrenIDs = serializer.ReadVec<EntityID>();

		transform.m_position = serializer.Read<Vec3f>();
		transform.m_scale = serializer.Read<Vec3f>();
		transform.m_rotation = serializer.Read<Quat>();
	}
}
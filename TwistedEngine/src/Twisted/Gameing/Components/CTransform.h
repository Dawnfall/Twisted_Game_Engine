#pragma once

#include "AppCore.h"
#include "Utils/Utils.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/Gameing/WorldRegistry.h"

namespace Twisted
{
	enum class TWISTED_API RelativeSpace
	{
		LOCAL,
		WORLD
	};

	class TWISTED_API CTransform :public AComponent
	{
	public:
		CTransform(Entity entity);

		void OnInit()override;
		void OnDestroy()override;

		//***************
		// Transform

		inline const Vec3f& GetLocalPosition() const { return m_position; }
		inline const Quat& GetLocalRotation() const { return m_rotation; }
		inline const Vec3f& GetLocalScale() const { return m_scale; }
		inline const Vec3f GetLocalRotationEulerRad() const { return glm::eulerAngles(m_rotation); }

		inline void SetLocalPosition(const Vec3f& newPositon) { m_position = newPositon; }
		inline void SetLocalRotation(const Quat& newRotation) { m_rotation = newRotation; }
		inline void SetLocalScale(const Vec3f& newScale) { m_scale = newScale; }
		inline void SetWorldPosition(const Vec3f& newPosition) { m_position = WorldToLocalPoint(newPosition); }

		inline const Vec3f GetWorldPosition() const { return LocalToWorldPoint(Constants::Vec3Zero); }
		const Quat GetWorldRotation() const; //TODO... not sure is correct	
		const Vec3f GetWorldScale() const;
		inline const Vec3f GetWorldRotationEuler() const { return glm::eulerAngles(GetWorldRotation()); }

		//***************
		// Tree

		inline bool HasParent()const { return m_parent != NullEntity; }

		Entity GetParentEntity()const { return Entity(m_parent, const_cast<World*>(m_entity.GetWorld())); }

		CTransform* GetParent();
		const CTransform* GetParent() const;

		CTransform& GetChild(size_t index);
		const CTransform& GetChild(size_t index)const;

		inline const std::vector<EntityID>& GetChildrenIDs()const { return m_children; }
		inline const size_t GetChildCount() const { return m_children.size(); }
		const size_t GetSiblingsIndex()const;
		const size_t GetSiblingsCount()const;

		void SetSiblingsIndex(size_t newIndex);
		void SetParent(CTransform* transform);
		void SetParent(CTransform* newParent, size_t index);
		bool IsDescendant(const CTransform* potentialChild)const;

		//**************
		// Transformation matrices

		inline Mat4x4f GetLocalTranslationMatrix()const { return glm::translate(glm::mat4(1.0f), GetLocalPosition()); }
		inline Mat4x4f GetLocalRotationMatrix()const { return glm::toMat4(GetLocalRotation()); }
		inline Mat4x4f GetLocalScaleMatrix()const { return glm::scale(glm::mat4(1.0f), GetLocalScale()); }
		inline Mat4x4f GetLocalModelMatrix()const { return	GetLocalTranslationMatrix() * GetLocalRotationMatrix() * GetLocalScaleMatrix(); }

		inline Mat4x4f GetInvertTranslationMatrix()const { return glm::translate(glm::mat4(1.0f), -GetLocalPosition()); }
		inline Mat4x4f GetInvertRotationMatrix() const { return glm::toMat4(glm::conjugate(GetLocalRotation())); }
		inline Mat4x4f GetInvertScaleMatrix() const { return glm::scale(glm::mat4(1.0f), 1.0f / GetLocalScale()); }
		inline Mat4x4f GetInvertLocalModelMatrix()const { return GetInvertScaleMatrix() * GetInvertRotationMatrix() * GetInvertTranslationMatrix(); }

		Mat4x4f GetWorldModelMatrix()const;

		Mat4x4f GetWorldInvertModelMatrix()const;

		//***************
		// Local <--> world

		inline Vec3f LocalToWorldVector(const Vec3f& vec)const
		{
			glm::vec4 worldVec = GetWorldModelMatrix() * glm::vec4(vec, 0.0f);
			return glm::vec3(worldVec.x, worldVec.y, worldVec.z);
		}
		inline Vec3f LocalToWorldPoint(const Vec3f& point)const
		{
			auto worldMat = GetWorldModelMatrix();
			auto pVec4 = glm::vec4(point, 1.0f);

			glm::vec4 worldPoint = GetWorldModelMatrix() * glm::vec4(point, 1.0f);
			return glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
		}
		inline Vec3f WorldToLocalVector(const Vec3f& vec)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix() * glm::vec4(vec, 0.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}
		inline Vec3f WorldToLocalPoint(const Vec3f& vec)const
		{
			glm::vec4 localVec = GetWorldInvertModelMatrix() * glm::vec4(vec, 1.0f);
			return glm::vec3(localVec.x, localVec.y, localVec.z);
		}

		inline Vec3f GetWorldForward()const { return LocalToWorldVector(Constants::Forward); }
		inline Vec3f GetWorldRight()const { return LocalToWorldVector(Constants::Right); }
		inline Vec3f GetWorldUp()const { return LocalToWorldVector(Constants::Up); }

		//****************
		// Transformations

		void Translate(const Vec3f& translation) { m_position += translation; }
		void Rotate(const Quat& rotation) { m_rotation = glm::normalize(m_rotation * rotation); }
		inline void Rotate(const Vec3f& eulerAnglesInDeg)
		{
			Quat rotationQuat = Quat(glm::radians(eulerAnglesInDeg));
			Rotate(rotationQuat);
		}
		inline void Scale(const Vec3f& scale) { m_scale *= scale; }
		inline void LookAt(const Vec3f& lookAtPoint, const Vec3f& upVector)
		{
			auto lookAtMat = glm::lookAt(GetLocalPosition(), lookAtPoint, upVector);
			auto newRotation = glm::quat_cast(lookAtMat);
			SetLocalRotation(newRotation);
		}

		//Serialization
		void Serialize(BinSerializer& buffer)const override;
		void Deserialize(BinSerializer& buffer)override;

		YAML::Node YamlSerialize() const override;
		void YamlDeserialize(const YAML::Node& node) override;

	private:
		void Unparent();

	private:

		Vec3f m_position;
		Quat m_rotation;
		Vec3f m_scale;

		EntityID m_parent;
		std::vector<EntityID> m_children;

		//inline static bool _registered = []() {
		//	ComponentRegistry::RegisterComponent<CTransform>();
		//	return true;
		//	}();
	};
}

REGISTER_COMPONENT(CTransform, "CTransform");




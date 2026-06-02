#pragma once

#include "AppCore.h"
#include "Utils/Utils.h"
#include "Utils/GlmUtils.h"
#include "AComponent.h"
#include "WorldRegistry.h"
#include "Utils/YamlUtils.h"
#include "Managers/RootTransformManager.h"

namespace Twisted
{
	enum class RelativeSpace
	{
		LOCAL,
		WORLD
	};

	class TWISTED_API TransformComponent : public AComponent
	{
		TCLASS_BODY()
	public:
		TransformComponent(Entity e) :
			AComponent(e)
		{
			RootManager = &GetWorld()->ForceGetManager<RootTransformManager>();
		}
		static void SetSiblingsIndex(TransformComponent& transform, size_t newIndex);
		static void SetParent(TransformComponent& transform, TransformComponent* newParent, RelativeSpace space = RelativeSpace::WORLD);
		inline void SetWorldPosition(const Vec3f& newPosition)
		{
			const TransformComponent* parent = GetParent();
			LocalPos = parent ? parent->WorldToLocalPoint(newPosition) : newPosition;
		}
		static void Unparent(TransformComponent& transform);

		//********************
		// Hierarchy

		inline bool HasParent()const { return Parent != NullEntity; }
		inline TransformComponent* GetParent()
		{
			return GetWorld()->TryGetComponent<TransformComponent>(Parent);
		}
		inline const TransformComponent* GetParent()const
		{
			return GetWorld()->TryGetComponent<TransformComponent>(Parent);
		}

		inline size_t GetSiblingsCount()const
		{
			const TransformComponent* parentTransform = GetParent();
			if (parentTransform)
				return parentTransform->GetChildCount();

			return RootManager->RootEntities.size();
		}
		inline size_t GetSiblingsIndex() const//TODO uncomment
		{
			const TransformComponent* parentTransform = GetParent();
			if (parentTransform)
			{
				auto it = std::find(parentTransform->Children.begin(), parentTransform->Children.end(), GetID());
				if (it != parentTransform->Children.end())
					return std::distance(parentTransform->Children.begin(), it);
			}

			auto& roots = RootManager->RootEntities;

			auto it = std::find(roots.begin(), roots.end(), GetID());
			if (it != roots.end())
				return std::distance(roots.begin(), it);

			TWISTED_ERROR("Unparented transform should be root entity: {}", std::to_string((int)GetID()));
			return 0;
		}

		inline const std::vector<EntityID>& GetChildrenIDs()const { return Children; }
		inline size_t GetChildCount() const { return Children.size(); }
		inline TransformComponent& GetChild(size_t index)	//assumes index in range	
		{
			return GetWorld()->GetComponent<TransformComponent>(Children[index]);
		}
		inline const TransformComponent& GetChild(size_t index)const//assumes index in range
		{
			return GetWorld()->GetComponent<TransformComponent>(Children[index]);
		}

		inline bool IsDescendant(const TransformComponent* potentialChild)const
		{
			while (potentialChild)
			{
				if (potentialChild == this)
					return true;
				potentialChild = potentialChild->GetParent();
			}
			return false;
		}

		//****************
		// Transformations

		inline void Translate(const Vec3f& translation) { LocalPos += translation; }
		inline void Rotate(const Quat& rotation) { LocalRot = glm::normalize(rotation * LocalRot); }
		inline void Rotate(const Vec3f& eulerAnglesInDeg)
		{
			Quat qx = glm::angleAxis(glm::radians(eulerAnglesInDeg.x), Constants::Right);
			Quat qy = glm::angleAxis(glm::radians(eulerAnglesInDeg.y), Constants::Up);
			Quat qz = glm::angleAxis(glm::radians(eulerAnglesInDeg.z), Constants::Forward);

			Rotate(qz * qy * qx);
		}
		inline void Scale(const Vec3f& scale) { LocalScale *= scale; }
		inline void LookAt(const Vec3f& lookAtPoint, const Vec3f& upVector)
		{
			Vec3f worldPos = GetWorldPosition();
			Quat worldRot = glm::conjugate(glm::quat_cast(glm::lookAt(worldPos, lookAtPoint, upVector)));
			const TransformComponent* parent = GetParent();
			LocalRot = parent ? glm::inverse(parent->GetWorldRotation()) * worldRot : worldRot;
		}

		inline Mat4x4f GetLocalTranslationMatrix()const { return glm::translate(glm::mat4(1.0f), LocalPos); }
		inline Mat4x4f GetLocalRotationMatrix()const { return glm::toMat4(LocalRot); }
		inline Mat4x4f GetLocalScaleMatrix()const { return glm::scale(glm::mat4(1.0f), LocalScale); }
		inline Mat4x4f GetLocalModelMatrix()const { return GetLocalTranslationMatrix() * GetLocalRotationMatrix() * GetLocalScaleMatrix(); }

		inline Mat4x4f GetInvertTranslationMatrix()const { return glm::translate(glm::mat4(1.0f), -LocalPos); }
		inline Mat4x4f GetInvertRotationMatrix()const { return glm::toMat4(glm::conjugate(LocalRot)); }
		inline Mat4x4f GetInvertScaleMatrix()const { return glm::scale(glm::mat4(1.0f), 1.0f / LocalScale); }
		inline Mat4x4f GetInvertLocalModelMatrix()const { return GetInvertScaleMatrix() * GetInvertRotationMatrix() * GetInvertTranslationMatrix(); }

		inline Mat4x4f GetWorldModelMatrix() const
		{
			const TransformComponent* parentTransform = GetParent();
			if (parentTransform)
				return  parentTransform->GetWorldModelMatrix() * GetLocalModelMatrix();
			return GetLocalModelMatrix();
		}
		inline Mat4x4f GetWorldInvertModelMatrix()const
		{
			const TransformComponent* parentTransform = GetParent();
			if (parentTransform)
				return GetInvertLocalModelMatrix() * parentTransform->GetWorldInvertModelMatrix();
			return GetInvertLocalModelMatrix();
		}

		inline Vec3f GetWorldPosition()const { return LocalToWorldPoint(Constants::Vec3Zero); }
		inline Vec3f GetWorldForward()const { return GetWorldRotation() * Constants::Forward; }
		inline Vec3f GetWorldRight()const { return GetWorldRotation() * Constants::Right; }
		inline Vec3f GetWorldUp()const { return GetWorldRotation() * Constants::Up; }

		inline Vec3f GetForward()const { return LocalRot * Constants::Forward; }
		inline Vec3f GetRight()const { return LocalRot * Constants::Right; }
		inline Vec3f GetUp()const { return LocalRot * Constants::Up; }

		
		inline Vec3f LocalToWorldVector(const Vec3f& vec)const
		{
			glm::vec4 worldVec = GetWorldModelMatrix() * glm::vec4(vec, 0.0f);
			return glm::vec3(worldVec.x, worldVec.y, worldVec.z);
		}
		inline Vec3f LocalToWorldPoint(const Vec3f& point)const
		{
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

		inline Quat GetWorldRotation() const //TODO... not sure is correct
		{
			const TransformComponent* parentTransform = GetParent();
			if (parentTransform)
				return parentTransform->GetWorldRotation() * LocalRot;
			return LocalRot;
		}
		inline Vec3f GetWorldRotationEuler() { return glm::eulerAngles(GetWorldRotation()); }
		inline Vec3f GetWorldScale()const
		{
			const TransformComponent* parentTransform = GetParent();
			if (parentTransform)
				return parentTransform->GetWorldScale() * LocalScale; //not sure correct
			return LocalScale;
		}

		inline Vec3f GetLocalRotationEulerRad() { return glm::eulerAngles(LocalRot); }

		//************
		// DATA

		TPROPERTY()
		Vec3f LocalPos = { 0.0f,0.0f,0.0f };
		TPROPERTY()
		Quat LocalRot = { 1.0f,0.0f,0.0f,0.0f };
		TPROPERTY()
		Vec3f LocalScale = { 1.0f,1.0f,1.0f };

		TPROPERTY()
		EntityID Parent = NullEntity;
		TPROPERTY()
		std::vector<EntityID> Children;
		RootTransformManager* RootManager = nullptr;
	};


	template<>
	inline void OnCreateComponent(TransformComponent& transform)
	{
		if (transform.Parent == NullEntity)
			transform.RootManager->RootEntities.emplace_back(transform.GetID());
		else
		{
			transform.GetParent()->Children.emplace_back(transform.GetID());
		}
	}

	template<>
	inline void OnDestroyComponent(TransformComponent& transform)
	{
		for (EntityID child : transform.GetChildrenIDs())
			transform.GetWorld()->DestroyEntity(child);

		transform.Unparent(transform);
	}

}





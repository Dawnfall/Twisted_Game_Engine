#pragma once

#include "AppCore.h"
#include "Utils/Utils.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "Utils/YamlUtils.h"
#include "Twisted/Gameing/Managers/RootTransformManager.h"

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
		CTransform(Entity entity) :
			AComponent(entity)
		{
			RootManager = &GetWorld()->ForceGetManager<RootTransformManager>();
		}
		static void SetSiblingsIndex(CTransform& transform, size_t newIndex);
		static void SetParent(CTransform& transform, CTransform* newParent);
		static void SetParent(CTransform& transform, CTransform* newParent, size_t index);
		inline void SetWorldPosition(CTransform& transform, const Vec3f& newPosition) { transform.LocalPos = transform.WorldToLocalPoint(newPosition); }
		static void Unparent(CTransform& transform);

		//********************
		// Hierarchy

		inline bool HasParent()const { return Parent != NullEntity; }
		inline CTransform* GetParent()
		{
			return GetWorld()->TryGetComponent<CTransform>(Parent);
		}
		inline const CTransform* GetParent()const
		{
			return GetWorld()->TryGetComponent<CTransform>(Parent);
		}

		inline size_t GetSiblingsCount()const
		{
			const CTransform* parentTransform = GetParent();
			if (parentTransform)
				return parentTransform->GetChildCount();

			return RootManager->RootEntities.size();
		}
		inline size_t GetSiblingsIndex() const//TODO uncomment
		{
			const CTransform* parentTransform = GetParent();
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
		inline CTransform& GetChild(size_t index)	//assumes index in range	
		{
			return GetWorld()->GetComponent<CTransform>(Children[index]);
		}
		inline const CTransform& GetChild(size_t index)const//assumes index in range
		{
			return GetWorld()->GetComponent<CTransform>(Children[index]);
		}

		inline bool IsDescendant(const CTransform* potentialChild)const
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
		inline void Rotate(const Quat& rotation) { LocalRot = glm::normalize(LocalRot * rotation); }
		inline void Rotate(const Vec3f& eulerAnglesInDeg)
		{
			Quat rotationQuat = Quat(glm::radians(eulerAnglesInDeg));
			Rotate(rotationQuat);
		}
		inline void Scale(const Vec3f& scale) { LocalScale *= scale; }
		inline void LookAt(const Vec3f& lookAtPoint, const Vec3f& upVector)
		{
			auto lookAtMat = glm::lookAt(LocalPos, lookAtPoint, upVector);
			auto newRotation = glm::quat_cast(lookAtMat);
			LocalRot = newRotation;
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
			const CTransform* parentTransform = GetParent();
			if (parentTransform)
				return  parentTransform->GetWorldModelMatrix() * GetLocalModelMatrix();
			return GetLocalModelMatrix();
		}
		inline Mat4x4f GetWorldInvertModelMatrix()const
		{
			const CTransform* parentTransform = GetParent();
			if (parentTransform)
				return GetInvertLocalModelMatrix() * parentTransform->GetWorldInvertModelMatrix();
			return GetInvertLocalModelMatrix();
		}

		inline Vec3f GetWorldPosition()const { return LocalToWorldPoint(Constants::Vec3Zero); }
		inline Vec3f GetWorldForward()const { return LocalToWorldVector(Constants::Forward); }
		inline Vec3f GetWorldRight()const { return LocalToWorldVector(Constants::Right); }
		inline Vec3f GetWorldUp()const { return LocalToWorldVector(Constants::Up); }

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

		inline Quat GetWorldRotation() const //TODO... not sure is correct
		{
			const CTransform* parentTransform = GetParent();
			if (parentTransform)
				return parentTransform->GetWorldRotation() * LocalRot;
			return LocalRot;
		}
		inline Vec3f GetWorldRotationEuler() { return glm::eulerAngles(GetWorldRotation()); }
		inline Vec3f GetWorldScale()const
		{
			const CTransform* parentTransform = GetParent();
			if (parentTransform)
				return parentTransform->GetWorldScale() * LocalScale; //not sure correct
			return LocalScale;
		}

		inline Vec3f GetLocalRotationEulerRad() { return glm::eulerAngles(LocalRot); }

		//************
		// DATA

		Vec3f LocalPos = { 0.0f,0.0f,0.0f };
		Quat LocalRot = { 1.0f,0.0f,0.0f,0.0f };
		Vec3f LocalScale = { 1.0f,1.0f,1.0f };

		EntityID Parent = NullEntity;
		std::vector<EntityID> Children;
		RootTransformManager* RootManager = nullptr;
	};


	template<>
	inline void OnCreateComponent(CTransform& transform)
	{
		TWISTED_INFO("Transform created");

		if (transform.Parent == NullEntity)
			transform.RootManager->RootEntities.emplace_back(transform.GetID());
		else
		{
			transform.GetParent()->Children.emplace_back(transform.GetID());
		}
	}

	template<>
	inline void OnDestroyComponent(CTransform& transform)
	{
		for (EntityID child : transform.GetChildrenIDs())
			transform.GetWorld()->DestroyEntity(child);

		transform.Unparent(transform);
	}

	template<>
	inline YAML::Node YamlSerialize<CTransform>(const CTransform& transform)
	{
		YAML::Node node;

		node["pos"] = transform.LocalPos;
		node["scale"] = transform.LocalScale;
		node["rot"] = transform.LocalRot;
		node["parent"] = transform.Parent;
		node["children"] = transform.Children;

		return node;
	}
	template<>
	inline void YamlDeserialize<CTransform>(CTransform& transform, const YAML::Node& node)
	{
		transform.LocalPos = node["pos"].as<Vec3f>();
		transform.LocalScale = node["scale"].as<Vec3f>();
		transform.LocalRot = node["rot"].as<Quat>();
		transform.Parent = node["parent"].as<EntityID>();
		transform.Children = node["children"].as<std::vector<EntityID>>();
	}
	template<typename T>
	inline void BinSerialize(const CTransform& transform, BinSerializer& buffer)
	{
		buffer.Write<Vec3f>(transform.LocalPos, nullptr);
		buffer.Write<Vec3f>(transform.LocalScale, nullptr);
		buffer.Write<Quat>(transform.LocalRot, nullptr);

		buffer.Write<EntityID>(transform.Parent, nullptr);
		buffer.Write<std::vector<EntityID>>(transform.Children, nullptr);
	}
	template<typename T>
	inline void BinDeserialize(CTransform& transform, BinSerializer& buffer)
	{
		transform.LocalPos = buffer.Read<Vec3f>(nullptr);
		transform.LocalScale = buffer.Read<Vec3f>(nullptr);
		transform.LocalRot = buffer.Read<Quat>(nullptr);

		transform.Parent = buffer.Read<EntityID>(transform.GetWorld());
		transform.Children = buffer.Read<std::vector<EntityID>>(transform.GetWorld());
	}
}

REGISTER_COMPONENT(CTransform, "CTransform");




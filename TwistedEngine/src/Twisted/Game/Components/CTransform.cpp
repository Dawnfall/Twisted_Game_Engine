#include "twistedpch.h"
#include "CTransform.h"

#include "Twisted/Game/World.h"
namespace Twisted
{
	std::vector<CTransform*> CTransform::m_rootTransforms;

	CTransform::CTransform(EntityID entityID, World* world) :
		AComponent(entityID, world),
		m_name("new object"),
		m_position(0.0f, 0.0f, 0.0f),
		m_rotation(1, 0, 0, 0),
		m_scale(1.0f, 1.0f, 1.0f),
		m_parentID(NullEntity)
	{
		//m_parentID = parentID;
		//if (parentID == NullEntity)
		//	m_rootTransforms.emplace_back(this);
		//else
		//{
		//	CTransform* parent = world->GetComponent<CTransform>(parentID);
		//	parent->m_childrenIDs.emplace_back(entityID);
		//}
	}

	void CTransform::SetParent(EntityID newParentID)
	{
		if (newParentID == m_parentID)
			return;

		if (m_parentID == NullEntity)
		{
			m_rootTransforms.erase(std::remove(m_rootTransforms.begin(), m_rootTransforms.end(), this), m_rootTransforms.end());
		}
		else
		{
			CTransform* currentParentTransform = m_world->GetComponent<CTransform>(m_parentID);
			std::remove(currentParentTransform->m_childrenIDs.begin(), currentParentTransform->m_childrenIDs.end(), m_entityID);
		}

		m_parentID = newParentID;
		if (m_parentID == NullEntity)
		{
			m_rootTransforms.emplace_back(this);
		}
		else
		{
			CTransform* newParentTransform = m_world->GetComponent<CTransform>(newParentID);
			if (newParentTransform != nullptr)
				newParentTransform->m_childrenIDs.emplace_back(m_entityID);
		}
	}



	//CTransform::CTransform(EntityID entity, const Vec3f& position, const Quat& rotation, const Vec3f& scale) :
	//	AComponent(entity),
	//	m_position(position),
	//	m_rotation(rotation),
	//	m_scale(scale),
	//	m_parentID(NullEntity)
	//	//m_children()
	//{
	//}

	const Quat CTransform::GetWorldRotation() const //TODO... not sure is correct
	{
		if (m_parentID != NullEntity)
			return m_world->GetComponent<CTransform>(m_parentID)->GetWorldRotation() * GetLocalRotation();
		return GetLocalRotation();
	}
	const Vec3f CTransform::GetWorldScale() const
	{
		if (m_parentID != NullEntity)
			return m_world->GetComponent<CTransform>(m_parentID)->GetWorldScale() * GetLocalScale(); //not sure correct
		return GetLocalScale();
	}

	Mat4x4f CTransform::GetWorldModelMatrix()const
	{
		if (m_parentID != NullEntity)
		{
			CTransform* parentTransform = m_world->GetComponent<CTransform>(m_parentID);
			return  parentTransform->GetWorldModelMatrix() * GetLocalModelMatrix();
		}
		return GetLocalModelMatrix();
	}

	Mat4x4f CTransform::GetWorldInvertModelMatrix()const
	{
		if (m_parentID != NullEntity)
		{
			CTransform* parentTransform = m_world->GetComponent<CTransform>(m_parentID);
			return GetInvertLocalModelMatrix() * parentTransform->GetWorldInvertModelMatrix();
		}
		return GetInvertLocalModelMatrix();
	}
}
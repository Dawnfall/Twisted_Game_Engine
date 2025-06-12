
#include "CTransform.h"

#include "Twisted/Gameing/World.h"
#include "Logger.h"

namespace Twisted
{
	CTransform::CTransform(EntityID entityID, World* world) :
		AComponent(entityID, world),
		m_position(0.0f, 0.0f, 0.0f),
		m_rotation(1, 0, 0, 0),
		m_scale(1.0f, 1.0f, 1.0f),
		m_parentID(NullEntity)
	{
		m_world->m_rootEntities.emplace_back(entityID);
	}

	void CTransform::SetParent(EntityID newParentID)
	{
		if (newParentID == m_parentID)
			return;

		if (IsDescendant(newParentID))
		{
			TWISTED_WARN("Reparent prevented! Circular parenting!");
			return;
		}

		if (m_parentID == NullEntity)
		{
			auto& vec = m_world->m_rootEntities;
			vec.erase(std::remove(vec.begin(), vec.end(), m_entityID), vec.end());
		}
		else
		{
			CTransform& currentParentTransform = m_world->GetComponent<CTransform>(m_parentID);
			auto& vec = currentParentTransform.m_childrenIDs;
			vec.erase(std::remove(vec.begin(), vec.end(), m_entityID), vec.end());
		}
		m_parentID = newParentID;
		if (m_parentID == NullEntity)
		{
			m_world->m_rootEntities.emplace_back(m_entityID);
		}
		else
		{
			CTransform& newParentTransform = m_world->GetComponent<CTransform>(newParentID);
			newParentTransform.m_childrenIDs.emplace_back(m_entityID);
		}
	}

	bool CTransform::IsDescendant(EntityID potentialChildID)const
	{
		if (m_entityID == potentialChildID)
			return true;

		for (const EntityID childID : m_childrenIDs)
		{
			const CTransform& child = m_world->GetComponent<CTransform>(childID);
			if (child.IsDescendant(potentialChildID))
				return true;
		}
		return false;
	}
	const unsigned int CTransform::GetSiblingsIndex()const
	{
		if (m_parentID == NullEntity)
		{
			for (unsigned int i = 0; i < m_world->m_rootEntities.size(); i++)
				if (m_world->m_rootEntities[i] == m_entityID)
					return i;
			return 0; //never here
		}
		CTransform& parentTransform = m_world->GetComponent<CTransform>(m_parentID);
		for (unsigned int i = 0; i < parentTransform.m_childrenIDs.size(); i++)
			if (parentTransform.m_childrenIDs[i] == m_entityID)
				return i;
		return 0;
	}

	void CTransform::SetSiblingsIndex(unsigned int newIndex)
	{
		unsigned int currentIndex = GetSiblingsIndex();

		if (m_parentID == NullEntity)
		{
			newIndex = std::min(newIndex, static_cast<unsigned int>(m_world->m_rootEntities.size() - 1));
			if (newIndex == currentIndex)
				return;
			Utils::MoveItemInVector(m_world->m_rootEntities, currentIndex, newIndex);
		}
		else
		{
			CTransform& parentTransform = m_world->GetComponent<CTransform>(m_parentID);
			newIndex = std::min(newIndex, static_cast<unsigned int>(parentTransform.GetChildCount() - 1));
			if (newIndex == currentIndex)
				return;
			Utils::MoveItemInVector(parentTransform.m_childrenIDs, currentIndex, newIndex);
		}
	}

	const size_t CTransform::GetSiblingsCount()const
	{
		if (m_parentID == NullEntity)
			return m_world->m_rootEntities.size();

		const CTransform& parentTransform = m_world->GetComponent<CTransform>(m_parentID);
		return parentTransform.GetChildCount();
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
			return m_world->GetComponent<CTransform>(m_parentID).GetWorldRotation() * GetLocalRotation();
		return GetLocalRotation();
	}
	const Vec3f CTransform::GetWorldScale() const
	{
		if (m_parentID != NullEntity)
			return m_world->GetComponent<CTransform>(m_parentID).GetWorldScale() * GetLocalScale(); //not sure correct
		return GetLocalScale();
	}

	Mat4x4f CTransform::GetWorldModelMatrix()const
	{
		if (m_parentID != NullEntity)
		{
			const CTransform& parentTransform = m_world->GetComponent<CTransform>(m_parentID);
			return  parentTransform.GetWorldModelMatrix() * GetLocalModelMatrix();
		}
		return GetLocalModelMatrix();
	}

	Mat4x4f CTransform::GetWorldInvertModelMatrix()const
	{
		if (m_parentID != NullEntity)
		{
			const CTransform& parentTransform = m_world->GetComponent<CTransform>(m_parentID);
			return GetInvertLocalModelMatrix() * parentTransform.GetWorldInvertModelMatrix();
		}
		return GetInvertLocalModelMatrix();
	}
}
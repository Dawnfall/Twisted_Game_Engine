
#include "CTransform.h"
#include "Twisted/TwistedMacros.h"

#include "Twisted/Gameing/World.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"
#include "Debug/Logger.h"

#include <algorithm>
#include "Twisted/Gameing/World.h"

namespace Twisted
{

	void CTransform::Init()
	{
		m_entity.GetWorld()->m_rootEntities.emplace_back(m_entity.GetID());
	}

	CTransform::~CTransform()
	{
		for (EntityID child : GetChildrenIDs())
			m_entity.GetWorld()->DestroyEntity(child);

		Unparent();
	}

	//up to user to prevent cycles
	//assumes valid transform(or nullptr) of the same world
	void CTransform::SetParent(CTransform* newParent)
	{
		if (!newParent && m_parent == NullEntity)
			return;
		if (newParent && newParent->GetEntity().GetID() == m_parent)
			return;

		if (IsDescendant(newParent))
		{
			TWISTED_WARN("Reparent prevented! Circular parenting!");
			return;
		}

		Unparent();

		m_parent = (newParent) ? newParent->GetEntity().GetID() : NullEntity;
		if (!newParent)
			m_entity.GetWorld()->m_rootEntities.emplace_back(m_entity.GetID());
		else
			newParent->m_children.emplace_back(m_entity.GetID());
	}

	void CTransform::SetParent(CTransform* newParent, size_t index)
	{
		//TODO....
	}


	CTransform* CTransform::GetParent()
	{
		return m_entity.GetWorld()->TryGetComponent<CTransform>(m_parent);
	}

	const CTransform* CTransform::GetParent()const
	{
		return m_entity.GetWorld()->TryGetComponent<CTransform>(m_parent);
	}

	//assumes index in range
	CTransform& CTransform::GetChild(size_t index)
	{
		return m_entity.GetWorld()->GetComponent<CTransform>(m_children[index]);
	}

	//assumes index in range
	const CTransform& CTransform::GetChild(size_t index)const
	{
		return m_entity.GetWorld()->GetComponent<CTransform>(m_children[index]);
	}

	bool CTransform::IsDescendant(const CTransform* potentialChild)const
	{
		while (potentialChild)
		{
			if (potentialChild == this)
				return true;
			potentialChild = potentialChild->GetParent();
		}
		return false;
	}

	const size_t CTransform::GetSiblingsIndex()const //TODO uncomment
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
		{
			auto it = std::find(parentTransform->m_children.begin(), parentTransform->m_children.end(), m_entity.GetID());
			if (it != parentTransform->m_children.end())
				return std::distance(parentTransform->m_children.begin(), it);
		}

		auto& roots = m_entity.GetWorld()->m_rootEntities;

		auto it = std::find(roots.begin(), roots.end(), m_entity.GetID());
		if (it != roots.end())
			return std::distance(roots.begin(), it);

		TWISTED_ERROR("Unparented transform should be root entity: " + std::to_string((int)m_entity.GetID()));
		return 0;
	}

	//assumes index is valid [0 , size-1] ; could be optimized
	void CTransform::SetSiblingsIndex(size_t newIndex)
	{
		size_t index = GetSiblingsIndex();
		if (newIndex == index)
			return;

		CTransform* parent = GetParent();
		if (parent)
			Utils::MoveItemInVector(parent->m_children, index, newIndex);
		else
			Utils::MoveItemInVector(m_entity.GetWorld()->m_rootEntities, index, newIndex);
	}

	const size_t CTransform::GetSiblingsCount()const
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
			return parentTransform->GetChildCount();

		return m_entity.GetWorld()->m_rootEntities.size();
	}

	const Quat CTransform::GetWorldRotation() const //TODO... not sure is correct
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
			return parentTransform->GetWorldRotation() * GetLocalRotation();
		return GetLocalRotation();
	}

	const Vec3f CTransform::GetWorldScale() const
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
			return parentTransform->GetWorldScale() * GetLocalScale(); //not sure correct
		return GetLocalScale();
	}

	Mat4x4f CTransform::GetWorldModelMatrix()const
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
			return  parentTransform->GetWorldModelMatrix() * GetLocalModelMatrix();
		return GetLocalModelMatrix();
	}

	Mat4x4f CTransform::GetWorldInvertModelMatrix()const
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
			return GetInvertLocalModelMatrix() * parentTransform->GetWorldInvertModelMatrix();
		return GetInvertLocalModelMatrix();
	}

	void CTransform::Unparent()
	{
		CTransform* currentParentTransform = GetParent();

		if (m_parent!=NullEntity)
		{
			auto it = std::find(currentParentTransform->m_children.begin(), currentParentTransform->m_children.end(), m_entity.GetID());
			if (it != currentParentTransform->m_children.end())
				currentParentTransform->m_children.erase(it);
		}
		else
		{
			auto it = std::find(m_entity.GetWorld()->m_rootEntities.begin(), m_entity.GetWorld()->m_rootEntities.end(), m_entity.GetID());
			if (it != m_entity.GetWorld()->m_rootEntities.end())
				m_entity.GetWorld()->m_rootEntities.erase(it);
		}
		m_parent = NullEntity;
	}

	//Serialization
	void CTransform::Serialize(BinSerializer& buffer)const
	{
		buffer.Write<Vec3f>(m_position, nullptr);
		buffer.Write<Vec3f>(m_scale, nullptr);
		buffer.Write<Quat>(m_rotation, nullptr);

		buffer.Write<EntityID>(m_parent, nullptr);
		buffer.Write<std::vector<EntityID>>(m_children, nullptr);
	}

	void CTransform::Deserialize(BinSerializer& buffer)
	{
		m_position = buffer.Read<Vec3f>(nullptr);
		m_scale = buffer.Read<Vec3f>(nullptr);
		m_rotation = buffer.Read<Quat>(nullptr);

		m_parent = buffer.Read<EntityID>(m_entity.GetWorld());
		m_children = buffer.Read<std::vector<EntityID>>(m_entity.GetWorld());
	}
}

REGISTER_COMPONENT(CTransform, "CTransform");

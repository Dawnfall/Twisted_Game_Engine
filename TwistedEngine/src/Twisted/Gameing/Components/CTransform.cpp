
#include "CTransform.h"
#include "Twisted/Gameing/World.h"
#include "Logger.h"

#include <algorithm>

namespace Twisted
{

	void CTransform::Init() 
	{
		m_world->GetRootEntities().emplace_back(m_entityID);
	}

	CTransform::~CTransform()
	{
		for (EntityID child : GetChildrenIDs())
			m_world->DestroyEntity(child);

		Unparent();
	}

	//up to user to prevent cycles
	//assumes valid transform(or nullptr) of the same world
	void CTransform::SetParent(CTransform* newParent)
	{
		if (!newParent && m_parent == NullEntity)
			return;
		if (newParent && newParent->GetEntityID() == m_parent)
			return;

		//if (IsDescendant(newParent))
		//{
		//	TWISTED_WARN("Reparent prevented! Circular parenting!");
		//	return;
		//}

		Unparent();

		m_parent = (newParent) ? newParent->GetEntityID() : NullEntity;
		if (!newParent)
			m_world->GetRootEntities().emplace_back(m_entityID);
		else
			newParent->m_children.emplace_back(m_entityID);
	}

	void CTransform::SetParent(CTransform* newParent, size_t index)
	{
		//TODO...
	}


	CTransform* CTransform::GetParent() { return  m_world->TryGetComponent<CTransform>(m_parent); }

	const CTransform* CTransform::GetParent()const { return m_world->TryGetComponent<CTransform>(m_parent); }

	//assumes index in range
	CTransform& CTransform::GetChild(size_t index)
	{
		return m_world->GetComponent<CTransform>(m_children[index]);
	}

	//assumes index in range
	const CTransform& CTransform::GetChild(size_t index)const
	{
		return m_world->GetComponent<CTransform>(m_children[index]);
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

	const size_t CTransform::GetSiblingsIndex()const
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
		{
			auto it = std::find(parentTransform->m_children.begin(), parentTransform->m_children.end(), m_entityID);
			if (it != parentTransform->m_children.end())
				return std::distance(parentTransform->m_children.begin(), it);
		}
		auto it = std::find(m_world->GetRootEntities().begin(), m_world->GetRootEntities().end(), m_entityID);
		if (it != m_world->GetRootEntities().end())
			return std::distance(m_world->GetRootEntities().begin(), it);

		TWISTED_ERROR("Unparented transform should be root entity" + std::to_string((int)m_entityID));
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
			Utils::MoveItemInVector(m_world->GetRootEntities(), index, newIndex);
	}

	const size_t CTransform::GetSiblingsCount()const
	{
		const CTransform* parentTransform = GetParent();
		if (parentTransform)
			return parentTransform->GetChildCount();

		return m_world->GetRootEntities().size();
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

		auto& vec = (currentParentTransform) ? currentParentTransform->m_children : m_world->GetRootEntities();

		auto it = std::find(vec.begin(), vec.end(), m_entityID);
		if (it != vec.end())
			vec.erase(it);
		m_parent = NullEntity;
	}

	//Serialization
	void CTransform::Serialize(BinSerializer& buffer, AssetsLayer* assetsLayer)const
	{
		buffer.Write<Vec3f>(m_position);
		buffer.Write<Vec3f>(m_scale);
		buffer.Write<Quat>(m_rotation);

		buffer.Write<EntityID>(m_parent);
		buffer.Write<std::vector<EntityID>>(m_children);
	}

	void CTransform::Deserialize(BinSerializer& buffer, AssetsLayer* assetsLayer)
	{
		m_position = buffer.Read<Vec3f>();
		m_scale = buffer.Read<Vec3f>();
		m_rotation = buffer.Read<Quat>();

		m_parent = buffer.Read<EntityID>();
		m_children = buffer.Read<std::vector<EntityID>>();
	}
}

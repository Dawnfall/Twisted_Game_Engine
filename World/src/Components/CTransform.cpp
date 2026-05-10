
#include "Components/CTransform.h"
#include "Debug/Logger.h"

#include <algorithm>

#include "Utils/Utils.h"
#include "Entity.h"

namespace Twisted
{
	//up to user to prevent cycles
	//assumes valid transform(or nullptr) of the same world
	void TransformComponent::SetParent(TransformComponent& transform, TransformComponent* newParent)
	{
		if (!newParent && transform.Parent == NullEntity)
			return;
		if (newParent && newParent->GetID() == transform.Parent)
			return;

		if (transform.IsDescendant(newParent))
		{
			TWISTED_WARN("Reparent prevented! Circular parenting!");
			return;
		}

		Unparent(transform);

		transform.Parent = (newParent) ? newParent->GetID() : NullEntity;

		if (!newParent)
			transform.RootManager->RootEntities.emplace_back(transform.GetID());
		else
			newParent->Children.emplace_back(transform.GetID());
	}

	void TransformComponent::SetParent(TransformComponent& transform, TransformComponent* newParent, size_t index)
	{
		(void)transform;
		(void)newParent;
		(void)index;
		//TODO....
	}

	//assumes index is valid [0 , size-1] ; could be optimized
	void TransformComponent::SetSiblingsIndex(TransformComponent& transform, size_t newIndex)
	{
		size_t index = transform.GetSiblingsIndex();
		if (newIndex == index)
			return;

		TransformComponent* parent = transform.GetParent();
		if (parent)
			Utils::MoveItemInVector(parent->Children, index, newIndex);
		else
			Utils::MoveItemInVector(transform.RootManager->RootEntities, index, newIndex);
	}

	void TransformComponent::Unparent(TransformComponent& transform)
	{
		TransformComponent* currentParentTransform = transform.GetParent();

		if (transform.Parent != NullEntity)
		{
			auto it = std::find(currentParentTransform->Children.begin(), currentParentTransform->Children.end(), transform.GetID());
			if (it != currentParentTransform->Children.end())
				currentParentTransform->Children.erase(it);
		}
		else
		{
			auto it = std::find(transform.RootManager->RootEntities.begin(), transform.RootManager->RootEntities.end(), transform.GetID());
			if (it != transform.RootManager->RootEntities.end())
				transform.RootManager->RootEntities.erase(it);
		}
		transform.Parent = NullEntity;
	}


}




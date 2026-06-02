#pragma once

#include "World.h"
#include "Entity.h"
#include "Components/CTransform.h"
#include "Components/CName.h"
#include "Components/CRenderer.h"
#include "ModelAsset.h"
#include "ImportedAsset.h"
#include "Mesh.h"
#include "Material.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace Twisted::Editor
{
	inline void InstantiateModelNode(const ModelNode& node, World* world, TransformComponent* parentTransform)
	{
		Mesh* mesh = dynamic_cast<Mesh*>(const_cast<TObject*>(node.mesh.GetObj()));
		bool hasMesh = mesh != nullptr;
		bool hasChildren = !node.children.empty();

		if (!hasMesh && !hasChildren)
			return;

		Entity entity = world->CreateNewEntity();

		NameComponent& nameComp = world->GetComponent<NameComponent>(entity.GetID());
		nameComp.Name = node.name.empty() ? "Model Node" : node.name;

		TransformComponent& transform = world->GetComponent<TransformComponent>(entity.GetID());
		if (parentTransform)
			TransformComponent::SetParent(transform, parentTransform);

		Vec3f scale, translation, skew;
		Vec4f perspective;
		Quat  rotation;
		glm::decompose(node.localTransform, scale, rotation, translation, skew, perspective);
		transform.LocalPos   = translation;
		transform.LocalRot   = rotation;
		transform.LocalScale = scale;

		if (hasMesh)
		{
			RendererComponent& renderer = world->AddComponent<RendererComponent>(entity.GetID());
			renderer.mesh = mesh;
			if (!node.materialSlots.empty())
				renderer.material = dynamic_cast<Material*>(const_cast<TObject*>(node.materialSlots[0].material.GetObj()));
		}

		for (const ModelNode& child : node.children)
			InstantiateModelNode(child, world, &transform);
	}

	inline void InstantiateAsset(ImportedAsset* asset, World* world, TransformComponent* parentTransform = nullptr)
	{
		if (!asset || !world)
			return;
		if (auto* modelAsset = dynamic_cast<ModelAsset*>(asset))
		{
			if (modelAsset->IsLoaded())
				InstantiateModelNode(modelAsset->GetRootNode(), world, parentTransform);
		}
	}
}

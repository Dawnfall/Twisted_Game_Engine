#include "editorpch.h"
#include "DetailsPanel.h"

#include "EditorApp.h"
#include "Twisted/Game/Components/CTransform.h"
#include "Twisted/Game/Components/CRenderer.h"

namespace Twisted::Editor
{
	void DetailsPanel::RenderContent(EditorApp* editor)
	{
		EntityID selectedEntity = editor->GetSelectedEntity();
		CTransform* transform = editor->GetWorld()->GetComponent<CTransform>(selectedEntity);
		RenderTransform(transform);
		CRenderer* renderer = editor->GetWorld()->GetComponent<CRenderer>(selectedEntity);
		RenderRenderer(renderer);
	}

	void DetailsPanel::RenderTransform(CTransform* transform)
	{
		if (transform)
		{
			// Position
			Vec3f position = transform->GetLocalPosition();
			if (ImGui::InputFloat3("Position", &position.x))
			{
				transform->SetLocalPosition(position);
			}
			// Rotation (Euler angles in degrees)
			Vec3f rotationEuler = transform->GetLocalRotationEulerRad() * (180.0f / 3.14159265359f); // Convert radians to degrees
			if (ImGui::InputFloat3("Rotation", &rotationEuler.x))
			{
				Vec3f rotationEulerRad = rotationEuler * (3.14159265359f / 180.0f); // Convert degrees back to radians
				transform->SetLocalRotation(Quat(rotationEulerRad));
			}
			// Scale
			Vec3f scale = transform->GetLocalScale();
			if (ImGui::InputFloat3("Scale", &scale.x))
			{
				transform->SetLocalScale(scale);
			}
		}
	}
	void DetailsPanel::RenderRenderer(CRenderer* renderer)
	{
		if (renderer)
		{

		}
	}
}
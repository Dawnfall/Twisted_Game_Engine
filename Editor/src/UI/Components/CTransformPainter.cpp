#include "CTransformPainter.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	void CTransformPainter::Paint(void* obj) 
	{
		CTransform* transform = static_cast<CTransform*>(obj);

		Vec3f position = transform->GetLocalPosition();
		Vec3f rotationEuler = glm::degrees(transform->GetLocalRotationEulerRad());
		Vec3f scale = transform->GetLocalScale();

		if (ImGui::BeginTable("TransformTable", 2))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 80.0f);
			ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthFixed, 240.0f);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Position");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::InputFloat3("##Position", &position.x))
				transform->SetLocalPosition(position);

			// --- Rotation row ---
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Rotation");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::InputFloat3("##Rotation", &rotationEuler.x))
			{
				Quat newRot(rotationEuler);
				transform->SetLocalRotation(glm::radians(rotationEuler));
			}

			// --- Scale row ---
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Scale");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::InputFloat3("##Scale", &scale.x))
				transform->SetLocalScale(scale);

			ImGui::EndTable();
		}
	}

}

REGISTER_COMPONENT_PAINTER(CTransformPainter, CTransform)

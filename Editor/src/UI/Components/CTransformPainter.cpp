#include "CTransformPainter.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	void CTransformPainter::Paint(void* obj)
	{
		CTransform* transform = static_cast<CTransform*>(obj);

		if (ImGui::BeginTable("TransformTable", 2))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 80.0f);
			ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthFixed, 240.0f);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Position");
			ImGui::TableSetColumnIndex(1);
			ImGui::InputFloat3("##Position", &transform->LocalPos.x);

			// --- Rotation row ---
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Rotation");
			ImGui::TableSetColumnIndex(1);

			Vec3f rotationEuler = glm::degrees(transform->GetLocalRotationEulerRad());
			if (ImGui::InputFloat3("##Rotation", &rotationEuler.x))
			{
				Quat newRot(rotationEuler);
				transform->LocalRot = glm::radians(rotationEuler);
			}

			// --- Scale row ---
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Scale");
			ImGui::TableSetColumnIndex(1);
			ImGui::InputFloat3("##Scale", &transform->LocalScale.x);

			ImGui::EndTable();
		}
	}

}

REGISTER_COMPONENT_PAINTER(CTransformPainter, CTransform)

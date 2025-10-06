#include "EntityPainter.h"
#include "EditorMacros.h"

#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"
#include "Twisted/Gameing/Entity.h"

namespace Twisted::Editor
{
	void EntityPainter::Paint(void* obj)
	{
		Entity* entity = static_cast<Entity*>(obj);

		std::string idLabel = "ID: " + std::to_string(static_cast<int>(entity->GetID()));
		float idWidth = ImGui::CalcTextSize(idLabel.c_str()).x;
		float idPosX = (ImGui::GetContentRegionAvail().x - idWidth) * 0.5f;
		if (idPosX > 0.0f)
			ImGui::SetCursorPosX(idPosX);
		ImGui::Text("%s", idLabel.c_str());

		ImGui::Separator();


		for (auto& compPainter : EditorRegistry::GetInstance().CompPainters)
		{
			void* component = compPainter->GetComponent(*entity);
			if (component)
			{
				std::string compName = compPainter->GetComponentName();

				float windowWidth = ImGui::GetContentRegionAvail().x;
				float labelWidth = ImGui::CalcTextSize(compName.c_str()).x;
				float labelPosX = (windowWidth - labelWidth) * 0.5f;
				if (labelPosX > 0.0f)
					ImGui::SetCursorPosX(labelPosX);
				ImGui::Text("%s", compName.c_str());

				compPainter->Paint(component);

				ImGui::NewLine();
				ImGui::Separator();
			}
		}

		//// --- AddComponent Button ---
		Im::CenterCursor(ADD_COMPONENT_TEXT);
		if (ImGui::Button(ADD_COMPONENT_TEXT.c_str()))
			ImGui::OpenPopup(ADD_COMPONENT_POPUP.c_str());

		if (ImGui::BeginPopup(ADD_COMPONENT_POPUP.c_str()))
		{
			for (auto& compPainter : EditorRegistry::GetInstance().CompPainters)
			{
				void* component = compPainter->GetComponent(*entity);
				if (!component)
				{
					if (ImGui::Selectable(compPainter->GetComponentName().c_str()))
					{
						compPainter->AddComponent(*entity);
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndPopup();
		}
	}
}

REGISTER_DETAILS_PAINTER(EntityPainter)

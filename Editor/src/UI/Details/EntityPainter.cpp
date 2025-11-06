#include "EntityPainter.h"

#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/Gameing/WorldRegistry.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	void EntityPainter::Paint(void* obj)
	{
		Entity* entity = static_cast<Entity*>(obj);
		if (!entity->IsValid())
			return;

		std::string idLabel = "ID: " + std::to_string(static_cast<int>(entity->GetID()));
		float idWidth = ImGui::CalcTextSize(idLabel.c_str()).x;
		float idPosX = (ImGui::GetContentRegionAvail().x - idWidth) * 0.5f;
		if (idPosX > 0.0f)
			ImGui::SetCursorPosX(idPosX);
		ImGui::Text("%s", idLabel.c_str());

		ImGui::Separator();

		WorldRegistry& reg = WorldRegistry::GetInstance();
		for (const auto& [name, entry] : reg.GetComponentEntries())
		{
			AComponent* comp = entry.GetComponentMethod(*entity);
			if (comp)
			{
				auto painter = EditorRegistry::GetInstance().GetComponentPainter(name);

				float windowWidth = ImGui::GetContentRegionAvail().x;
				float labelWidth = ImGui::CalcTextSize(name.c_str()).x;
				float labelPosX = (windowWidth - labelWidth) * 0.5f;
				if (labelPosX > 0.0f)
					ImGui::SetCursorPosX(labelPosX);
				ImGui::Text("%s", name.c_str());

				painter->Paint(comp);

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
			WorldRegistry& worldReg = WorldRegistry::GetInstance();
			auto& entries = worldReg.GetComponentEntries();
			for (auto& entry : entries)
			{
				if (!entry.second.HasComponentMethod(*entity))
				{
					if (ImGui::Selectable(entry.first.c_str()))
					{
						entry.second.AddComponentMethod(*entity);
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndPopup();
		}
	}
}

REGISTER_DETAILS_PAINTER(Entity, EntityPainter)

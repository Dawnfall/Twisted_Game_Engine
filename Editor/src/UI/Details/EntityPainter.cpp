#include "UI/Details/EntityPainter.h"

#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"
#include "Entity.h"
#include "WorldRegistry.h"
#include "EditorApp/EditorRegistry.h"

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
		std::string pendingRemove;
		for (const auto& [name, entry] : reg.GetComponentEntries())
		{
			AComponent* comp = entry.GetComponentMethod(*entity);
			auto painter = EditorRegistry::GetInstance().GetComponentPainter(name);
			if (comp && painter)
			{
				float windowWidth = ImGui::GetContentRegionAvail().x;
				float labelWidth = ImGui::CalcTextSize(name.c_str()).x;
				float labelPosX = (windowWidth - labelWidth) * 0.5f;
				if (labelPosX > 0.0f)
					ImGui::SetCursorPosX(labelPosX);
				ImGui::Text("%s", name.c_str());

				if (entry.RemoveComponentMethod)
				{
					ImGui::SameLine();
					std::string removeLabel = "X##remove_" + name;
					if (ImGui::SmallButton(removeLabel.c_str()))
						pendingRemove = name;
				}

				painter->Paint(comp);

				ImGui::NewLine();
				ImGui::Separator();
			}
		}
		if (!pendingRemove.empty())
		{
			auto& removeEntry = reg.GetComponentEntries().at(pendingRemove);
			removeEntry.RemoveComponentMethod(*entity);
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


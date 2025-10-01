#include "DetailsPanel.h"

#include "EditorLayer.h"

#include "Selection.h"
#include "Twisted/Gameing/World.h"
#include "EditorRegistry.h"
#include "UI/Details/AssetPainter.h"
#include <variant>
#include <type_traits>
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
	void DetailsPanel::PaintContent()
	{
		std::visit([this](auto&& arg)
			{
				using SelectionType = std::decay_t<decltype(arg)>;
				if constexpr (std::is_base_of_v<SelectionType, std::unordered_set<EntityID>>)
				{
					if (arg.size() == 1)
					{
						World* gameWorld = m_editor->GetGameWorld();
						PaintEntity(*arg.begin(), gameWorld);
					}
				}
				//else if constexpr (std::is_base_of_v<AssetInfo, T>)
				//{
				//	AssetPainter assetPainter;
				//	assetPainter.Paint(arg);
				//}
				//else if constexpr (std::is_same_v<std::monostate, T>)
				//{

				//}
			}, Selection::GetInstance().GetSelection());
	}

	void DetailsPanel::PaintEntity(EntityID id, World* world)
	{
		std::string idLabel = "ID: " + std::to_string(static_cast<int>(id));
		float idWidth = ImGui::CalcTextSize(idLabel.c_str()).x;
		float idPosX = (ImGui::GetContentRegionAvail().x - idWidth) * 0.5f;
		if (idPosX > 0.0f)
			ImGui::SetCursorPosX(idPosX);
		ImGui::Text("%s", idLabel.c_str());

		ImGui::Separator();


		for (auto& compPainter : EditorRegistry::GetInstance().CompPainters)
		{
			void* component = compPainter->GetComponent(id, world);
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
				void* component = compPainter->GetComponent(id, world);
				if (!component)
				{
					if (ImGui::Selectable(compPainter->GetComponentName().c_str()))
					{
						compPainter->AddComponent(id, world);
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndPopup();
		}
	}
}
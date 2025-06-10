#include "DetailsPanel.h"

#include "UI/Details/CNameRenderer.h"
#include "UI/Details/CTransformRenderer.h"
#include "EditorRuntime.h"

namespace Twisted::Editor
{
	void DetailsPanel::RenderContent()
	{
		auto world = m_editor->GetGameWorld();
		if (!world || m_editor->GetSelectedEntity() == NullEntity)
			return;

		// Centered entity ID
		std::string idLabel = "ID: " + std::to_string(static_cast<int>(m_editor->GetSelectedEntity()));
		float idWidth = ImGui::CalcTextSize(idLabel.c_str()).x;
		float idPosX = (ImGui::GetContentRegionAvail().x - idWidth) * 0.5f;
		if (idPosX > 0.0f)
			ImGui::SetCursorPosX(idPosX);
		ImGui::Text("%s", idLabel.c_str());

		ImGui::Separator();
		RenderComponent<CName>();
		RenderComponent<CTransform>();


		// --- AddComponent Button ---
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponentPopup");

		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			//// Example: List of available components
			//if (!world->HasComponent<CRenderer>(editor->GetSelectedEntity()))
			//{
			//	if (ImGui::Selectable("CRenderer"))
			//	{
			//		world->AddComponents<CRenderer>(editor->GetSelectedEntity());
			//		ImGui::CloseCurrentPopup();
			//	}
			//}
			// Add more components here as needed

			ImGui::EndPopup();
		}
	}
}
#include "WorldSystemPanel.h"
#include "EditorData/EditorData.h"
#include "Utils/Utils.h"
#include "UI/ImguiExtensions.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	const std::string ADD_SYSTEM_TEXT = "Add system";
	const std::string ADD_SYSTEM_POPUP = "system_popup";
	void WorldSystemPanel::PaintContent()
	{
		auto gameWorld = EditorData::GetInstance().GetGameWorld();
		
		auto& systems = gameWorld->GetAllSystems();
		for (auto& sys : systems)
		{
			std::string className = Utils::GetObjTypeName(sys.get());
			ImGui::Text("%s", className.c_str());
		}


		// --- AddComponent Button ---
		Im::CenterCursor(ADD_SYSTEM_TEXT);
		if (ImGui::Button(ADD_SYSTEM_TEXT.c_str()))
			ImGui::OpenPopup(ADD_SYSTEM_POPUP.c_str());

		if (ImGui::BeginPopup(ADD_SYSTEM_POPUP.c_str()))
		{
			/*for (auto& compPainter : SystemReg::GetInstance().Sys)
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
			}*/
			ImGui::EndPopup();
		}
	}
}

REGISTER_EDITOR_PANEL(WorldSystemPanel)
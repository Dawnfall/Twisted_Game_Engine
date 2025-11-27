#include "WorldSystemPanel.h"
#include "Utils/Utils.h"
#include "UI/ImguiExtensions.h"
#include "EditorApp/EditorRegistry.h"
#include "EditorApp/EditorWorldService.h"
#include "Twisted/Gameing/GameService.h"
#include "Twisted/Gameing/World.h"

namespace Twisted::Editor
{
	const std::string ADD_SYSTEM_TEXT = "Add system";
	const std::string ADD_SYSTEM_POPUP = "system_popup";
	void WorldSystemPanel::PaintContent()
	{
		auto gameWorld = GameService::GetInstance()->GameWorld;
		
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
			for (auto& [sysName,entry] : WorldRegistry::GetInstance().GetSystemEntries())
			{
				bool hasSystem = entry.HasSystemMethod(*gameWorld);
				if (!hasSystem)
				{
					if (ImGui::Selectable(sysName.c_str()))
					{
						entry.AddSystemMethod(*gameWorld);
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndPopup();
		}
	}
}

REGISTER_EDITOR_PANEL(WorldSystemPanel)

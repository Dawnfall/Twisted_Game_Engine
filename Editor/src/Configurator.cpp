#include "editorpch.h"
#include "Configurator.h"
#include "EditorConfig.h"

namespace Twisted::Editor
{
	void Configurator::LoadConfig()
	{
		//Twisted::XmlSerializer ser = Twisted::XmlSerializer::LoadFromFile(editorRootFolder / editorConfigFile);
		//Data = std::make_shared<EditorConfig>();

		//ser.Read(WIN_TITLE_NAME, Data->windowTitle);
		//ser.Read(WIN_SIZE_NAME, Data->windowSize);
		//ser.Read(WIN_POS_NAME, Data->windowPos);

		//ser.Read(IMGUI_SIZE, Data->imguiSize);
		//ser.Read(IMGUI_DATA, Data->imguiData);
		//ImGui::LoadIniSettingsFromMemory(m_config->imguiData, m_config->imguiSize);
	}


	void Configurator::WriteConfig() const
	{
		//Twisted::XmlSerializer ser;
		//ser.Write(WIN_TITLE_NAME, editor->GetWindow()->GetTitle());
		//ser.Write(WIN_SIZE_NAME, editor->GetWindow()->GetSize());
		//ser.Write(WIN_POS_NAME, editor->GetWindow()->GetPosition());

		//Data->imguiData = ImGui::SaveIniSettingsToMemory(&Data->imguiSize);
		//ser.Write(IMGUI_SIZE, Data->imguiSize);
		//ser.Write(IMGUI_DATA, Data->imguiData);

		//Twisted::XmlSerializer::SaveToFile(editorRootFolder / editorConfigFile, ser);
	}
}
#include "pch.h"
#include "Editor.h"

namespace Twisted::Editor
{
	bool m_showDemo = true;
	void Editor::Init(Application& app)
	{
		InitImgui(app);
	}

	void Editor::Update(Application& app)
	{
		ImGui::ShowDemoWindow(&m_showDemo);
	}

	void Editor::Terminate(Application& app)
	{
		TerminateImgui(app);
	}

	void Editor::InitImgui(Application& app)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(app.WindowManager.GetWindow()->Pointer, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();
	}

	void Editor::TerminateImgui(Application& app)
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}
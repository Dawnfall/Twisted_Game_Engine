#include "pch.h"

#include "Editor.h"
#include "Twisted/Application.h"
#include "Windows/TreeWindow.h"

namespace Twisted::Editor
{
	void Editor::Init(Twisted::Application* app)
	{
			//Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
			ImGui::StyleColorsDark();
			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(app->Windows.GetWindow()->Pointer, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
			ImGui_ImplOpenGL3_Init();

			OpenNewWindow<TreeWindow>();
	}

	void Editor::Update(Twisted::Application* app)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (auto& window : m_openedWindows)
		{
			window->Render(app);
		}
	}

	void Editor::Render(Twisted::Application* app)
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Editor::Terminate(Twisted::Application* app)
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}

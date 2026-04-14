#include "ImguiExtensions.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Twisted/Windowing/Window.h"

namespace Im
{
	void Init(Twisted::Window* window)
	{
		static bool isInit = false;
		if (isInit) return;
		isInit = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->GetRawPointer());
		// install_callbacks=true chains our engine callbacks already set on the window
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::GetIO().IniFilename = nullptr;
		ImGui::StyleColorsDark();
	}

	void StartFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Terminate()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

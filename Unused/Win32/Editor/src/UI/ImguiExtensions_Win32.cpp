#include "ImguiExtensions.h"

#include <Windows.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <ImGuizmo.h>

#include "Window.h"
#include "WindowsService.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Im
{
	void Init(Twisted::Window* window)
	{
		static bool isInit = false;
		if (isInit) return;
		isInit = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		HWND hwnd = static_cast<HWND>(window->GetRawPointer());
		ImGui_ImplWin32_Init(hwnd);

		window->GetWindowsService()->GetPollMsgEvent().AddListener([](void* rawMsg) {
			MSG* msg = static_cast<MSG*>(rawMsg);
			ImGui_ImplWin32_WndProcHandler(msg->hwnd, msg->message, msg->wParam, msg->lParam);
		});

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
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void Terminate()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

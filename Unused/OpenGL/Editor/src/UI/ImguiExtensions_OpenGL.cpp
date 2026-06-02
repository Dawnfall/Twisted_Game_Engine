#include "ImguiExtensions.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

#include "Texture.h"
#include "OpenGL/Texture_OpenGL.h"

namespace Im
{
	ImTextureID GetImGuiTextureID(const Twisted::Texture* tex)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(tex->GetBackend()->TexID));
	}

	void Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

#include "ImguiExtensions.h"

#include "AppCore.h"
#include "Debug/Logger.h"

#ifdef NATIVE_USE

#include <Windows.h>
#include <backends/imgui_impl_opengl3.h>
//#include <backends/imgui_impl_win32.h>

namespace Im
{
	void Init(void* windowPointer)
	{
		HWND hwnd = static_cast<HWND>(windowPointer);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		//ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplOpenGL3_Init("#version 460"); // You can use another version string depending on your context

		Im::SetFlags();
		Im::SetStyle();

		ImGui::GetIO().IniFilename = nullptr;
	}

	void StartFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

	}

	void Terminate()
	{
		ImGui_ImplOpenGL3_Shutdown();
		//ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

#else

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace Im
{
	void Init(void* windowPointer)
	{
		if (!glfwInit()) //due to globals and dlls glfw is not initialized outside of dll
		{
			TWISTED_ERROR("GLFW init failure; RenderCore Init failure!"); //TODO: editor output
		}

		//Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(windowPointer), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();

		Im::SetFlags();
		Im::SetStyle();

		ImGui::GetIO().IniFilename = nullptr;
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
#endif


namespace Im
{
	float ICON_SIZE()
	{
		return ImGui::GetFont()->FontSize + 3.0f;
	}

	bool PathBox(const char* label, std::filesystem::path& path, char* pathBuffer, ImVec2 size_arg) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		bool ret = false;
		const ImGuiID id = window->GetID(label);
		int* state = window->StateStorage.GetIntRef(id, 0);

		ImGui::SameLine();

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 uiPos = ImGui::GetCursorPos();
		ImVec2 size = ImGui::CalcItemSize(size_arg, 200, GUI_ELEMENT_SIZE());
		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

		// buttons
		if (!(*state & 0b001)) {
			ImGui::PushClipRect(bb.Min, bb.Max, false);

			// background
			bool hovered = g.IO.MousePos.x >= bb.Min.x && g.IO.MousePos.x <= bb.Max.x &&
				g.IO.MousePos.y >= bb.Min.y && g.IO.MousePos.y <= bb.Max.y;
			bool clicked = hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
			bool anyOtherHC = false; // are any other items hovered or clicked?
			window->DrawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[(*state & 0b10) ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg]));

			// fetch the buttons (so that we can throw some away if needed)
			std::vector<std::string> btnList;
			float totalWidth = 0.0f;
			for (auto comp : path) {
				std::string section = comp.string();
				if (section.size() == 1 && (section[0] == '\\' || section[0] == '/'))
					continue;

				totalWidth += ImGui::CalcTextSize(section.c_str()).x + style.FramePadding.x * 2.0f + GUI_ELEMENT_SIZE();
				btnList.push_back(section);
			}
			totalWidth -= GUI_ELEMENT_SIZE();

			// UI buttons
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, ImGui::GetStyle().ItemSpacing.y));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			bool isFirstElement = true;
			for (size_t i = 0; i < btnList.size(); i++) {
				if (totalWidth > size.x - 30 && i != btnList.size() - 1) { // trim some buttons if there's not enough space
					float elSize = ImGui::CalcTextSize(btnList[i].c_str()).x + style.FramePadding.x * 2.0f + GUI_ELEMENT_SIZE();
					totalWidth -= elSize;
					continue;
				}

				ImGui::PushID(static_cast<int>(i));
				if (!isFirstElement) {
					ImGui::ArrowButtonEx("##dir_dropdown", ImGuiDir_Right, ImVec2(GUI_ELEMENT_SIZE(), GUI_ELEMENT_SIZE()));
					anyOtherHC |= ImGui::IsItemHovered() | ImGui::IsItemClicked();
					ImGui::SameLine();
				}
				if (ImGui::Button(btnList[i].c_str(), ImVec2(0, GUI_ELEMENT_SIZE()))) {

					std::string newPath = "";

					for (size_t j = 0; j <= i; j++) {
						newPath += btnList[j];

						if (j != i)
							newPath += "\\";

					}
					path = std::filesystem::u8path(newPath);
					ret = true;
				}
				anyOtherHC |= ImGui::IsItemHovered() | ImGui::IsItemClicked();
				ImGui::SameLine();
				ImGui::PopID();

				isFirstElement = false;
			}
			ImGui::PopStyleVar(2);


			// click state
			if (!anyOtherHC && clicked) {
				strcpy(pathBuffer, path.string().c_str());
				*state |= 0b001;
				*state &= 0b011; // remove SetKeyboardFocus flag
			}
			else
				*state &= 0b110;

			// hover state
			if (!anyOtherHC && hovered && !clicked)
				*state |= 0b010;
			else
				*state &= 0b101;

			ImGui::PopClipRect();

			// allocate space
			ImGui::SetCursorPos(uiPos);
			ImGui::ItemSize(size);
		}
		// input box
		else {
			bool skipActiveCheck = false;
			if (!(*state & 0b100)) {
				skipActiveCheck = true;
				ImGui::SetKeyboardFocusHere();
				if (!ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					*state |= 0b100;
			}
			if (ImGui::InputTextEx("##pathbox_input", "", pathBuffer, 1024, size_arg, ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::string tempStr(pathBuffer);
				if (std::filesystem::exists(tempStr))
					path = std::filesystem::u8path(tempStr);
				ret = true;
			}
			if (!skipActiveCheck && !ImGui::IsItemActive())
				*state &= 0b010;
		}

		return ret;
	}
	bool FavoriteButton(const char* label, bool isFavorite)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		ImVec2 pos = window->DC.CursorPos;
		bool ret = ImGui::InvisibleButton(label, ImVec2(GUI_ELEMENT_SIZE(), GUI_ELEMENT_SIZE()));

		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();

		float size = g.LastItemData.Rect.Max.x - g.LastItemData.Rect.Min.x;

		int numPoints = 5;
		float innerRadius = size / 4;
		float outerRadius = size / 2;
		float angle = PI / numPoints;
		ImVec2 center = ImVec2(pos.x + size / 2, pos.y + size / 2);

		// fill
		if (isFavorite || hovered || active) {
			ImU32 fillColor = 0xff00ffff;// ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);
			if (hovered || active)
				fillColor = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]);

			// since there is no PathFillConcave, fill first the inner part, then the triangles
			// inner
			window->DrawList->PathClear();
			for (int i = 1; i < numPoints * 2; i += 2)
				window->DrawList->PathLineTo(ImVec2(center.x + innerRadius * sin(i * angle), center.y - innerRadius * cos(i * angle)));
			window->DrawList->PathFillConvex(fillColor);

			// triangles
			for (int i = 0; i < numPoints; i++) {
				window->DrawList->PathClear();

				int pIndex = i * 2;
				window->DrawList->PathLineTo(ImVec2(center.x + outerRadius * sin(pIndex * angle), center.y - outerRadius * cos(pIndex * angle)));
				window->DrawList->PathLineTo(ImVec2(center.x + innerRadius * sin((pIndex + 1) * angle), center.y - innerRadius * cos((pIndex + 1) * angle)));
				window->DrawList->PathLineTo(ImVec2(center.x + innerRadius * sin((pIndex - 1) * angle), center.y - innerRadius * cos((pIndex - 1) * angle)));

				window->DrawList->PathFillConvex(fillColor);
			}
		}

		// outline
		window->DrawList->PathClear();
		for (int i = 0; i < numPoints * 2; i++) {
			float radius = i & 1 ? innerRadius : outerRadius;
			window->DrawList->PathLineTo(ImVec2(center.x + radius * sin(i * angle), center.y - radius * cos(i * angle)));
		}
		window->DrawList->PathStroke(ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]), true, 2.0f);

		return ret;
	}
	bool FolderNode(const char* label, ImTextureID icon, bool& clicked)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		clicked = false;

		ImU32 id = window->GetID(label);
		int opened = window->StateStorage.GetInt(id, 0);
		ImVec2 pos = window->DC.CursorPos;
		const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= pos.x && g.IO.MousePos.x < pos.x + g.FontSize);
		if (ImGui::InvisibleButton(label, ImVec2(-FLT_MIN, g.FontSize + g.Style.FramePadding.y * 2)))
		{
			if (is_mouse_x_over_arrow) {
				int* p_opened = window->StateStorage.GetIntRef(id, 0);
				opened = *p_opened = !*p_opened;
			}
			else {
				clicked = true;
			}
		}
		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		bool doubleClick = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
		if (doubleClick && hovered) {
			int* p_opened = window->StateStorage.GetIntRef(id, 0);
			opened = *p_opened = !*p_opened;
			clicked = false;
		}
		if (hovered || active)
			window->DrawList->AddRectFilled(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered]));

		// Icon, text
		float icon_posX = pos.x + g.FontSize + g.Style.FramePadding.y;
		float text_posX = icon_posX + g.Style.FramePadding.y + ICON_SIZE();
		ImGui::RenderArrow(window->DrawList, ImVec2(pos.x, pos.y + g.Style.FramePadding.y), ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[((hovered && is_mouse_x_over_arrow) || opened) ? ImGuiCol_Text : ImGuiCol_TextDisabled]), opened ? ImGuiDir_Down : ImGuiDir_Right);
		window->DrawList->AddImage(icon, ImVec2(icon_posX, pos.y), ImVec2(icon_posX + ICON_SIZE(), pos.y + ICON_SIZE()));
		ImGui::RenderText(ImVec2(text_posX, pos.y + g.Style.FramePadding.y), label);
		if (opened)
			ImGui::TreePush(label);
		return opened != 0;
	}
	bool FileIcon(const char* label, bool isSelected, ImTextureID icon, ImVec2 size)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		float windowSpace = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		ImVec2 pos = window->DC.CursorPos;
		bool ret = false;

		if (ImGui::InvisibleButton(label, size))
			ret = true;

		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		bool doubleClick = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
		if (doubleClick && hovered)
			ret = true;

		float iconSize = size.y - g.FontSize * 2;
		float iconPosX = pos.x + (size.x - iconSize) / 2.0f;
		ImVec2 textSize = ImGui::CalcTextSize(label, 0, true, size.x);

		if (hovered || active || isSelected)
			window->DrawList->AddRectFilled(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[active ? ImGuiCol_HeaderActive : (isSelected ? ImGuiCol_Header : ImGuiCol_HeaderHovered)]));

		//if (hasPreview) {
		//	ImVec2 availSize = ImVec2(size.x, iconSize);

		//	float scale = std::min<float>(availSize.x / previewWidth, availSize.y / previewHeight);
		//	availSize.x = previewWidth * scale;
		//	availSize.y = previewHeight * scale;

		//	float previewPosX = pos.x + (size.x - availSize.x) / 2.0f;
		//	float previewPosY = pos.y + (iconSize - availSize.y) / 2.0f;

		//	window->DrawList->AddImage(icon, ImVec2(previewPosX, previewPosY), ImVec2(previewPosX + availSize.x, previewPosY + availSize.y));
		//}
		//else
		window->DrawList->AddImage(icon, ImVec2(iconPosX, pos.y), ImVec2(iconPosX + iconSize, pos.y + iconSize));

		window->DrawList->AddText(g.Font, g.FontSize, ImVec2(pos.x + (size.x - textSize.x) / 2.0f, pos.y + iconSize), ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]), label, 0, size.x);


		float lastButtomPos = ImGui::GetItemRectMax().x;
		float thisButtonPos = lastButtomPos + style.ItemSpacing.x + size.x; // Expected position if next button was on same line
		if (thisButtonPos < windowSpace)
			ImGui::SameLine();

		return ret;
	}

	void CenterCursor(const std::string& text)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		float padding = ImGui::GetStyle().FramePadding.x * 2.0f;
		float widgetWidth = textSize.x + padding;

		// total available region in current window/column
		float regionWidth = ImGui::GetContentRegionAvail().x;

		// center by moving cursor before the button
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (regionWidth - widgetWidth) * 0.5f);
	}



	void SetFlags()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch+
	}

	void SetStyle()
	{
		ImGui::StyleColorsDark();
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

	bool InputText(InputTextToken& token)
	{
		if (token.PreLabel != "")
		{
			ImGui::TextUnformatted(token.PreLabel.c_str());
			ImGui::SameLine();
		}

		static std::vector<char> buffer;
		buffer.assign(token.Text.begin(), token.Text.end());
		if (buffer.size() < MAX_INPUT_SIZE)
			buffer.resize(MAX_INPUT_SIZE, '\0');
		else
			buffer[MAX_INPUT_SIZE - 1] = '\0'; // Ensure null-termination

		if (token.JustCreated && token.DoAutoFocus)
		{
			ImGui::SetKeyboardFocusHere();
		}

		bool changed = ImGui::InputText("##1", buffer.data(), MAX_INPUT_SIZE, token.Flags);
		if (changed)
			token.Text = buffer.data();

		if (token.JustCreated)
		{
			token.PrevIsActive = ImGui::IsItemActive();
		}
		else
		{
			bool isActive = ImGui::IsItemActive();
			token.IsLostFocus = token.PrevIsActive && !isActive;
			token.PrevIsActive = isActive;
		}

		ImGui::SameLine();

		if (token.PostLabel != "")
		{
			ImGui::TextUnformatted(token.PostLabel.c_str());
			ImGui::SameLine();
		}

		token.JustCreated = false;
		return changed;
	}
}

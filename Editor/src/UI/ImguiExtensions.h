#pragma once
#define NOMINMAX
#include <algorithm>
#include <filesystem>

//#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_internal.h"

#include "AppCore.h"
#include "Twisted/Gameing/AComponent.h"
#include "Twisted/RegisterLayer/ObjectID.h"

namespace Im
{
	struct InputTextToken
	{
		std::string PreLabel;
		std::string PostLabel;
		std::string Text;
		ImGuiInputTextFlags Flags = 0;
		bool DoAutoFocus = false;

		bool JustCreated = true;
		bool PrevIsActive = false;
		bool IsLostFocus = false;
	};

	struct AssetEntryToken
	{
		std::filesystem::path Path;
		bool IsSelected = false;
		ImVec4 HighLightColor;
		ImVec2 CellSize;
	};

	const ImU32 GREEN_COLOR = IM_COL32(100, 255, 100, 255);
	const ImU32 RED_COLOR = IM_COL32(255, 100, 100, 255);
	const float PI = 3.141592f;
	const int DEFAULT_ICON_SIZE = 32;
	constexpr int MAX_INPUT_SIZE = 256;

	constexpr float GUI_ELEMENT_SIZE()
	{
		return std::max<float>(GImGui->FontSize + 10.0f, 24.0f);
	}
	float ICON_SIZE();

	//void Init(GLFWwindow* windowPointer);
	void Init(void* windowPointer);

	void SetFlags();
	void SetStyle();
	void Terminate();
	void Render();

	void StartFrame();
	void EndFrame();

	//from filebrowser
	bool PathBox(const char* label, std::filesystem::path& path, char* pathBuffer, ImVec2 size_arg);
	bool FavoriteButton(const char* label, bool isFavorite);
	bool FolderNode(const char* label, ImTextureID icon, bool& clicked);
	bool FileIcon(const char* label, bool isSelected, ImTextureID icon, ImVec2 size);

	void CenterCursor(const std::string& text);

	// to use string
	bool InputText(InputTextToken& token);

	bool AssetEntry(AssetEntryToken& token);

	inline void SetLayoutIniFile(const std::string& filePath, bool force = false)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.IniFilename = (filePath == "") ? nullptr : filePath.c_str();

		if (force)
			ImGui::LoadIniSettingsFromDisk(io.IniFilename);

	}

	template<typename T>
	Twisted::EntityID ComponentDropField(const std::string& label, Twisted::EntityID& entity, Twisted::World* world)
	{
		static_assert(std::is_base_of_v<Twisted::AComponent, T>, "T must derive from AComponent");

		ImGui::Text("%s", label.c_str());
		ImGui::SameLine();

		// Reserve space
		//T* component = world->TryGetComponent<T>(entity);

		//ImVec2 size = ImVec2(150, ImGui::GetTextLineHeightWithSpacing());
		//ImVec2 min = ImGui::GetCursorScreenPos();
		//ImVec2 max = ImVec2(min.x + size.x, min.y + size.y);

		//// Draw background (Unity-like field box)
		//ImU32 bgCol = ImGui::GetColorU32(ImGuiCol_FrameBg);
		//ImGui::GetWindowDrawList()->AddRectFilled(min, max, bgCol, 3.0f);
		//ImGui::GetWindowDrawList()->AddRect(min, max, ImGui::GetColorU32(ImGuiCol_Border), 3.0f);

		//// Draw the text centered vertically
		// ImGui::SetCursorScreenPos(ImVec2(min.x + 5, min.y + (size.y - ImGui::GetTextLineHeight()) * 0.5f));
		// ImGui::TextUnformatted(displayName.c_str());

		//// Create an invisible button over the whole area for hover detection
		//ImGui::SetCursorScreenPos(min);
		//ImGui::InvisibleButton("##dropfield", size);

		// Change cursor when hovered
		//if (ImGui::IsItemHovered())
		//{
		//	// Default hover cursor when nothing is dragged
		//	ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

		//	if (const ImGuiPayload* payload = ImGui::GetDragDropPayload())
		//	{
		//		bool isValidDrag = strcmp(payload->DataType, payloadType) == 0;
		//		if (isValidDrag) // make sure it's a component payload
		//		{
		//			T* payloadData = static_cast<Twisted::AComponent*>(payload->Data);
		//			T* dropped = dynamic_cast<T*>(payloadData);
		//			
		//			if (dropped)
		//			{
		//				// valid payload type
		//				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand); // "approve" look
		//				ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), GREEN_COLOR);
		//			}
		//			else
		//			{
		//				// invalid payload
		//				ImGui::SetMouseCursor(ImGuiMouseCursor_NotAllowed);
		//				ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), RED_COLOR);
		//			}
		//		}
		//	}
		//}

		//if (ImGui::BeginDragDropTarget())
		//{
		//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadType))
		//	{
		//		T* dropped = dynamic_cast<T*>(payload->Data);
		//		component = dropped;
		//	}
		//	ImGui::EndDragDropTarget();
		//}
		//return component;

		return entity;
	}

	template<typename T>
	Twisted::ObjectID ObjectDropField(const char* label, Twisted::ObjectID& obj, const char* payloadType)
	{
		static_assert(std::is_base_of_v<Twisted::BaseObject, T>, "T must derive from BaseObject");

		ImGui::Text("%s", label);
		ImGui::SameLine();

		ImGui::PushID(&obj);  // unique ID based on member address

		// Display a button or empty slot
		std::string buttonLabel;
		if (obj)
			buttonLabel = "Object " + std::to_string(obj.GetID());
		else
			buttonLabel = "<None>";
		ImGui::Button(buttonLabel.c_str(), ImVec2(150, 0));

		// --- Drag source (so user can drag it elsewhere) ---
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(payloadType, &obj, sizeof(Twisted::ObjectID));
			ImGui::Text("Dragging %u", std::to_string(obj.GetID()));
			ImGui::EndDragDropSource();
		}


		ImGui::PopID();
		return obj;
	}
}

//	if (ImGui::BeginDragDropTarget())
//	{
//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadType))
//		{
//			T* dropped = *(T**)payload->Data;
//			ptr = dropped;
//		}
//		ImGui::EndDragDropTarget();
//	}
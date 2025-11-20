#pragma once
#define NOMINMAX

//#include "AppCore.h"
#include "imgui.h"
#include "imgui_internal.h"

//#include "Twisted/Gameing/AComponent.h"
#include "Twisted/TObject.h"
//#include "Twisted/Gameing/World.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/Windowing/Window.h"
#include "EditorConstants.h"

#include "Twisted/AssetsLayer/AssetInfo.h"
#include "Twisted/AssetsLayer/AssetsLayer.h"

#include <filesystem>
//#include <algorithm>
#include <string>
//#include <vector>
#include <typeindex>
#include <tuple>
#include <type_traits>

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

	void Init(Twisted::Window* window);
	void Terminate();

	void StartFrame();
	void EndFrame();

	void Render();

	// Drag / Drop
	template<typename T>
	void DragSource(const std::string& type, T& obj, const std::string& dragText)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(type.c_str(), &obj, sizeof(T));
			ImGui::Text(dragText.c_str());
			ImGui::EndDragDropSource();
		}
	}

	template<typename T>
	std::pair<bool, T> DragTarget(const std::string& dropType, T defaultValue)
	{
		bool result = false;
		T returnValue = defaultValue;

		bool isDraggedOver = ImGui::BeginDragDropTarget();
		if (!isDraggedOver)
			return { false,defaultValue };

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dropType.c_str());
		if (!payload)
		{
			ImGui::EndDragDropTarget();
			return { false,defaultValue };
		}

		T obj = *static_cast<const T*>(payload->Data);
		ImGui::EndDragDropTarget();
		return { true, obj };
	}

	template<typename T>
	std::pair<bool, T> DragTarget(const std::vector<std::string>& allowedDropTypes, T defaultValue)
	{
		for (std::string& dropType : allowedDropTypes)
		{
			auto result = DragTarget<T>(dropType, defaultValue);
			if (result.first)
				return result.second;
		}
		return defaultValue;
	}

	bool DrawToggle(const char* label, bool& value);

	//from filebrowser
	bool PathBox(const char* label, std::filesystem::path& path, char* pathBuffer, ImVec2 size_arg);
	bool FavoriteButton(const char* label, bool isFavorite);
	bool FolderNode(const char* label, ImTextureID icon, bool& clicked);
	bool FileIcon(const char* label, bool isSelected, ImTextureID icon, ImVec2 size);

	void CenterCursor(const std::string& text);

	// to use string
	bool InputText(InputTextToken& token);

	inline void SetLayoutIniFile(const std::string& filePath, bool force = false)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.IniFilename = (filePath == "") ? nullptr : filePath.c_str();

		if (force)
			ImGui::LoadIniSettingsFromDisk(io.IniFilename);
	}

	template<typename T>
	std::pair<bool, T*> ObjectPicker(const std::string& label, const std::string& uniqueID)
	{
		bool isSuccess = false;
		T* obj = nullptr;

		const std::string buttonID = "...##" + uniqueID;
		const std::string popupID = "ObjPickerPopup##" + uniqueID;

		if (ImGui::Button(buttonID.c_str(), ImVec2(25, 0)))
			ImGui::OpenPopup(popupID.c_str());

		if (ImGui::BeginPopup(popupID.c_str()))
		{
			ImGui::Text(label.c_str());
			ImGui::Separator();

			std::vector<T*> objects = Twisted::AssetsLayer::GetInstance()->GetObjectsOfType<T>();
			for (T* assetObject : objects)
			{
				if (ImGui::Selectable(assetObject->GetName().c_str()))
				{
					obj = assetObject;
					isSuccess = true;
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		return { isSuccess,obj };
	}

	template<typename T>
	bool ComponentDropField(const std::string& label, Twisted::Entity& entity)
	{
		static_assert(std::is_base_of_v<Twisted::AComponent, T>, "T must derive from AComponent");

		//ImGui::Text("%s", label.c_str());
		//ImGui::SameLine();

		//// Reserve space
		//T* component = world->TryGetComponent<T>(entity);

		//ImVec2 size = ImVec2(150, ImGui::GetTextLineHeightWithSpacing());
		//ImVec2 min = ImGui::GetCursorScreenPos();
		//ImVec2 max = ImVec2(min.x + size.x, min.y + size.y);

		//// Draw background (Unity-like field box)
		//ImU32 bgCol = ImGui::GetColorU32(ImGuiCol_FrameBg);
		//ImGui::GetWindowDrawList()->AddRectFilled(min, max, bgCol, 3.0f);
		//ImGui::GetWindowDrawList()->AddRect(min, max, ImGui::GetColorU32(ImGuiCol_Border), 3.0f);

		//// Draw the text centered vertically
		//ImGui::SetCursorScreenPos(ImVec2(min.x + 5, min.y + (size.y - ImGui::GetTextLineHeight()) * 0.5f));
		//ImGui::TextUnformatted(displayName.c_str());

		//// Create an invisible button over the whole area for hover detection
		//ImGui::SetCursorScreenPos(min);
		//ImGui::InvisibleButton("##dropfield", size);

		//// Change cursor when hovered
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
		return false;
	}

	template<typename T>
	T* ObjectDropField(const std::string& label, T*& obj)
	{
		static_assert(std::is_base_of_v<Twisted::TObject, T>, "T must derive from TObject");

		ImGui::TextUnformatted(label.c_str());
		ImGui::SameLine();

		//drop area
		std::string dropFieldText = obj ? obj->GetName() : "None";
		dropFieldText += "##" + label;
		ImGui::Button(dropFieldText.c_str(), ImVec2(150, 0)); // fixed-width dummy
		auto objResult = Im::DragTarget<T*>(Twisted::Editor::Constants::OBJECT_DRAG_TYPE, nullptr);

		if (objResult.first)
		{
			obj = objResult.second;
		}
		else
		{
			auto [success, assetInfo] = Im::DragTarget<Twisted::AssetInfo*>(Twisted::Editor::Constants::ASSET_DRAG_TYPE, nullptr);
			if (success && assetInfo)
			{
				auto& objects = Twisted::AssetsLayer::GetInstance()->GetManagedAssetObjects(assetInfo);
				if(!objects.empty())
					obj = dynamic_cast<T*>(objects[0].GetObj());
			}
		}

		ImGui::SameLine();

		auto [success, selectObj] = ObjectPicker<T>("Select an object", label);
		if (success)
			obj = selectObj;

		return obj;
	}
}

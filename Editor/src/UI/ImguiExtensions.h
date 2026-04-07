#pragma once
#define NOMINMAX

//#include "AppCore.h"
#include "imgui.h"
#include "imgui_internal.h"

//#include "Twisted/Gameing/AComponent.h"
//#include "Twisted/Gameing/World.h"
#include "Twisted/TObject.h"
#include "Twisted/Gameing/Entity.h"
#include "Twisted/Windowing/Window.h"
#include "Twisted/Rendering/Texture.h"
#include "EditorConstants.h"

#include "Twisted/AssetsLayer/AssetInfo.h"
#include "Twisted/AssetsLayer/AssetsService.h"
#include "Twisted/Application/Application.h"
//#include <algorithm>
//#include <vector>
#include <filesystem>
#include <string>
#include <type_traits>
#include <utility>
#include "Twisted/Gameing/AComponent.h"
#include <algorithm>
#include <vector>

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
	constexpr float OBJECT_FIELD_WIDTH = 150.0f;

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
	ImTextureID GetImGuiTextureID(const Twisted::Texture* tex);

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
		//T returnValue = defaultValue;

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
		for (const std::string& dropType : allowedDropTypes)
		{
			auto result = DragTarget<T>(dropType, defaultValue);
			if (result.first)
				return result;
		}
		return { false, defaultValue };
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

			Twisted::AssetsService* assets = Twisted::Application::GetInstance().GetService<Twisted::AssetsService>();
			std::vector<T*> objects = assets->GetObjectsOfType<T>();
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

		ImGui::TextUnformatted(label.c_str());
		ImGui::SameLine();

		bool hasComponent = entity && entity.GetWorld()->template TryGetComponent<T>(entity.GetID());
		const char* text = hasComponent ? "Entity" : "None";
		ImGui::PushID(label.c_str());
		ImGui::Button(text, ImVec2(OBJECT_FIELD_WIDTH, 0));

		bool changed = false;
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Twisted::Editor::Constants::ENTITY_DRAG_TYPE.c_str()))
			{
				Twisted::Entity dropped = *static_cast<Twisted::Entity*>(payload->Data);
				if (dropped && dropped.GetWorld()->template TryGetComponent<T>(dropped.GetID()))
				{
					entity = dropped;
					changed = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
		return changed;
	}

	template<typename T>
	T* ObjectDropField(const std::string& label, T* obj)
	{
		static_assert(std::is_base_of_v<Twisted::TObject, T>, "T must derive from TObject");

		ImGui::TextUnformatted(label.c_str());
		ImGui::SameLine();

		std::string text = obj ? obj->GetName() : "None";
		ImGui::PushID(label.c_str());
		ImGui::Button(text.c_str(), ImVec2(OBJECT_FIELD_WIDTH, 0));

		// Single Begin/End drag drop block — calling BeginDragDropTarget twice on the same item is invalid
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* objPayload = ImGui::AcceptDragDropPayload(Twisted::Editor::Constants::OBJECT_DRAG_TYPE.c_str()))
			{
				obj = *static_cast<T**>(objPayload->Data);
			}
			else if (const ImGuiPayload* assetPayload = ImGui::AcceptDragDropPayload(Twisted::Editor::Constants::ASSET_DRAG_TYPE.c_str()))
			{
				Twisted::AssetInfo* assetInfo = *static_cast<Twisted::AssetInfo**>(assetPayload->Data);
				if (assetInfo)
				{
					auto& objects = Twisted::Application::GetInstance().GetService<Twisted::AssetsService>()->GetManagedAssetObjects(assetInfo);
					if (!objects.empty())
						obj = dynamic_cast<T*>(objects[0].GetObj());
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		auto [success, selectObj] = ObjectPicker<T>("Select an object", label);
		if (success)
			obj = selectObj;

		ImGui::PopID();
		return obj;
	}


	//ImTextureID GetImGuiTextureID(Twisted::Texture* ) { return ImTextureID{}; }

}

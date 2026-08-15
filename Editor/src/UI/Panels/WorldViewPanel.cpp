#include "UI/Panels/WorldViewPanel.h"

#include "EditorApp/EditorRegistry.h"
#include "UI/ImguiExtensions.h"

#include "FrameBuffer.h"
#include "Components/CCamera.h"
#include "Components/CTransform.h"
#include "Managers/CameraManager.h"
#include "GameService.h"
#include "SceneInstantiator.h"
#include "EditorConstants.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel() :EditorPanel("World View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			if (!m_editorService) return;
			auto* camera = m_editorService->GetEditorWorld()->ForceGetManager<CameraManager>().GetMainCamera();
			if (!camera) return;
			// Don't resize the FB here — the command buffer may already reference its image views.
			// Store the desired size and apply it in PreRender() before BeginFrame().
			m_pendingFbSize = Size;
			if (Size.y > 0)
				camera->AspectRatio = (float)Size.x / (float)Size.y;
		});
	}

	void WorldViewPanel::PreRender()
	{
		if (m_pendingFbSize.x <= 0 || m_pendingFbSize.y <= 0) return;
		if (!m_editorService) return;
		auto* camera = m_editorService->GetEditorWorld()->ForceGetManager<CameraManager>().GetMainCamera();
		if (!camera) return;
		if (auto* fb = camera->Fb.get())
			fb->SetSize(m_pendingFbSize);
		m_pendingFbSize = { 0, 0 };
	}
	  
	void WorldViewPanel::SetTool(GizmoTool tool)
	{
		m_activeTool = tool;
		switch (tool)
		{
		case GizmoTool::Move:      m_gizmoOperation = ImGuizmo::TRANSLATE; break;
		case GizmoTool::Rotate:    m_gizmoOperation = ImGuizmo::ROTATE;    break;
		case GizmoTool::Scale:     m_gizmoOperation = ImGuizmo::SCALE;     break;
		case GizmoTool::Universal: m_gizmoOperation = ImGuizmo::UNIVERSAL; break;
		default: break; // View — gizmo is simply not drawn
		}
	}

	void WorldViewPanel::HandleShortcuts()
	{
		if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
			return;
		if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
			return;
		if (ImGui::IsKeyPressed(ImGuiKey_Q, false)) SetTool(GizmoTool::View);
		if (ImGui::IsKeyPressed(ImGuiKey_W, false)) SetTool(GizmoTool::Move);
		if (ImGui::IsKeyPressed(ImGuiKey_E, false)) SetTool(GizmoTool::Rotate);
		if (ImGui::IsKeyPressed(ImGuiKey_R, false)) SetTool(GizmoTool::Scale);
		if (ImGui::IsKeyPressed(ImGuiKey_Y, false)) SetTool(GizmoTool::Universal);
		if (ImGui::IsKeyPressed(ImGuiKey_X, false)) m_localSpace = !m_localSpace;
	}

	void WorldViewPanel::PaintContent()
	{
		HandleShortcuts();

		if (!m_editorService)
			return;

		CameraComponent* camera = m_editorService->GetEditorWorld()->ForceGetManager<CameraManager>().GetMainCamera();
		if (!camera)
			return;

		Framebuffer* fb = camera->Fb.get();
		if (!fb || !fb->IsValid() || !fb->GetColor())
			return;

		ImVec2 panelPos = ImGui::GetCursorScreenPos();
		m_editorService->WorldViewRect = { panelPos.x, panelPos.y, panelPos.x + Size.x, panelPos.y + Size.y };

		ImGui::Image(
			Im::GetImGuiTextureID(fb->GetColor().get()),
			ImVec2((float)Size.x, (float)Size.y),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);

		// Handle asset drops into the scene
		{
			auto [dropped, droppedAsset] = Im::DragTarget<ImportedAsset*>(Constants::ASSET_DRAG_TYPE, nullptr);
			if (dropped && droppedAsset)
			{
				World* dropWorld = Application::GetInstance().GetService<GameService>()->GetGameWorld();
				if (dropWorld)
				{
					if (dynamic_cast<ModelAsset*>(droppedAsset))
					{
						InstantiateAsset(droppedAsset, dropWorld, nullptr);
					}
					else if (auto* matAsset = dynamic_cast<MaterialAsset*>(droppedAsset))
					{
						Material* mat = dynamic_cast<Material*>(matAsset->GetPrimaryObject());
						if (mat)
						{
							for (const Entity& entity : m_editorService->GetSelection().GetSelectedEntities())
							{
								if (RendererComponent* renderer = dropWorld->TryGetComponent<RendererComponent>(entity.GetID()))
									renderer->material = mat;
							}
						}
					}
				}
			}
		}

		// View orientation gizmo (top-right corner)
		Mat4x4f view = camera->GetViewMatrix();

		float viewArray[16];
		memcpy(viewArray, glm::value_ptr(view), sizeof(viewArray));

		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		ImGuizmo::SetRect(panelPos.x, panelPos.y, (float)Size.x, (float)Size.y);

		constexpr float gizmoSize = 100.0f;
		ImVec2 gizmoPos = { panelPos.x + Size.x - gizmoSize, panelPos.y };

		float viewBefore[16];
		memcpy(viewBefore, viewArray, sizeof(viewArray));

		ImGuizmo::ViewManipulate(viewArray, 8.0f, gizmoPos, ImVec2(gizmoSize, gizmoSize), 0x10101010);

		if (memcmp(viewArray, viewBefore, sizeof(viewArray)) != 0)
		{
			Mat4x4f newView = glm::make_mat4(viewArray);
			TransformComponent& transform = m_editorService->GetEditorWorld()->GetComponent<TransformComponent>(camera->GetID());
			transform.LocalPos = Vec3f(glm::inverse(newView)[3]);

			// glm::lookAt row 0 = cross(forward, up)  = -physicalRight
			// glm::lookAt row 2 = -forward
			// In GLM column-major: row i = (V[0][i], V[1][i], V[2][i])
			Vec3f physRight = -Vec3f(newView[0][0], newView[1][0], newView[2][0]);
			Vec3f up        =  Vec3f(newView[0][1], newView[1][1], newView[2][1]);
			Vec3f forward   = -Vec3f(newView[0][2], newView[1][2], newView[2][2]);
			Mat3x3f rotMat;
			rotMat[0] = physRight;
			rotMat[1] = up;
			rotMat[2] = forward;
			transform.LocalRot = glm::quat_cast(rotMat);
		}

		World* gameWorld = Application::GetInstance().GetService<GameService>()->GetGameWorld();
		const auto& selectedEntities = m_editorService->GetSelection().GetSelectedEntities();
		if (gameWorld && !selectedEntities.empty() && m_activeTool != GizmoTool::View)
		{
			Mat4x4f proj = camera->GetProjectionMatrix();
			float projArray[16];
			memcpy(projArray, glm::value_ptr(proj), sizeof(projArray));

			ImGuizmo::MODE space = (m_gizmoOperation == ImGuizmo::SCALE || m_localSpace) ? ImGuizmo::LOCAL : ImGuizmo::WORLD;

			for (const Entity& entity : selectedEntities)
			{
				TransformComponent* transform = gameWorld->TryGetComponent<TransformComponent>(entity.GetID());
				if (!transform)
					continue;

				Mat4x4f worldMatrix = transform->GetWorldModelMatrix();
				float modelArray[16];
				memcpy(modelArray, glm::value_ptr(worldMatrix), sizeof(modelArray));

				if (ImGuizmo::Manipulate(viewArray, projArray, m_gizmoOperation, space, modelArray))
				{
					Mat4x4f m = glm::make_mat4(modelArray);

					Vec3f newPos = Vec3f(m[3]);

					// Extract scale from column lengths; a zero-scale column is degenerate —
					// rotation cannot be recovered from it, so fall back to the existing rotation.
					Vec3f newScale = {
						glm::length(Vec3f(m[0])),
						glm::length(Vec3f(m[1])),
						glm::length(Vec3f(m[2]))
					};

					constexpr float kMinScale = 1e-6f;
					const bool canExtractRot =
						newScale.x > kMinScale &&
						newScale.y > kMinScale &&
						newScale.z > kMinScale;

					Quat newWorldRot = transform->GetWorldRotation();
					if (canExtractRot)
					{
						Mat3x3f rotMat;
						rotMat[0] = Vec3f(m[0]) / newScale.x;
						rotMat[1] = Vec3f(m[1]) / newScale.y;
						rotMat[2] = Vec3f(m[2]) / newScale.z;
						newWorldRot = glm::quat_cast(rotMat);
					}

					const TransformComponent* parent = transform->GetParent();
					Vec3f parentWorldScale = parent ? parent->GetWorldScale() : Vec3f(1.0f);

					transform->LocalPos   = parent ? parent->WorldToLocalPoint(newPos) : newPos;
					transform->LocalRot   = parent ? glm::inverse(parent->GetWorldRotation()) * newWorldRot : newWorldRot;
					transform->LocalScale = {
						parentWorldScale.x > kMinScale ? newScale.x / parentWorldScale.x : transform->LocalScale.x,
						parentWorldScale.y > kMinScale ? newScale.y / parentWorldScale.y : transform->LocalScale.y,
						parentWorldScale.z > kMinScale ? newScale.z / parentWorldScale.z : transform->LocalScale.z,
					};
				}
			}
		}

		PaintToolbar(panelPos);
	}
	void WorldViewPanel::PaintToolbar(ImVec2 panelPos)
	{
		constexpr float toolbarH = 26.0f;
		constexpr float btnW     = 26.0f;
		constexpr float padding  = 3.0f;

		ImDrawList* dl = ImGui::GetWindowDrawList();

		if (!m_toolbarCollapsed)
		{
			dl->AddRectFilled(
				{ panelPos.x,          panelPos.y },
				{ panelPos.x + Size.x, panelPos.y + toolbarH },
				IM_COL32(22, 22, 22, 210)
			);

			float btnY = panelPos.y + (toolbarH - ImGui::GetFrameHeight()) * 0.5f;
			ImGui::SetCursorScreenPos({ panelPos.x + padding, btnY });

			auto ToolBtn = [&](const char* label, const char* tooltip, GizmoTool tool)
			{
				bool active = (m_activeTool == tool);
				if (active)
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
				if (ImGui::Button(label, { btnW, 0 }))
					SetTool(tool);
				if (active)
					ImGui::PopStyleColor();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", tooltip);
			};

			ToolBtn("H##gz",  "View (Q)",          GizmoTool::View);      ImGui::SameLine(0, 2);
			ToolBtn("T##gz",  "Move (W)",           GizmoTool::Move);      ImGui::SameLine(0, 2);
			ToolBtn("R##gz",  "Rotate (E)",         GizmoTool::Rotate);    ImGui::SameLine(0, 2);
			ToolBtn("S##gz",  "Scale (R)",          GizmoTool::Scale);     ImGui::SameLine(0, 2);
			ToolBtn("U##gz",  "Universal (Y)",      GizmoTool::Universal); ImGui::SameLine(0, 8);

			{
				bool local = m_localSpace;
				if (local)
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
				if (ImGui::Button(local ? "L##space" : "G##space", { btnW, 0 }))
					m_localSpace = !m_localSpace;
				if (local)
					ImGui::PopStyleColor();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(local ? "Local space (X)" : "Global space (X)");
			}

			ImGui::SameLine(0, 16);
			{
				bool dc = m_editorService->DrawColliders;
				if (dc)
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
				if (ImGui::Button("C##coll", { btnW, 0 }))
					m_editorService->DrawColliders = !m_editorService->DrawColliders;
				if (dc)
					ImGui::PopStyleColor();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Draw Colliders");
			}

			ImGui::SetCursorScreenPos({ panelPos.x + Size.x - btnW - padding, btnY });
			if (ImGui::Button("<<##gz_col", { btnW, 0 }))
				m_toolbarCollapsed = true;
		}
		else
		{
			ImGui::SetCursorScreenPos({ panelPos.x + padding, panelPos.y + padding });
			if (ImGui::Button(">>##gz_exp", { btnW, 0 }))
				m_toolbarCollapsed = false;
		}
	}
}

REGISTER_EDITOR_PANEL(WorldViewPanel)

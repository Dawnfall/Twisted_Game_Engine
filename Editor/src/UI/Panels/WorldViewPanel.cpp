#include "WorldViewPanel.h"

#include "EditorApp/EditorRegistry.h"

#include "Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h"
#include "Twisted/Gameing/Entity.h"
#include "Utils/GlmUtils.h"
#include "UI/EditorPanel.h"
#include "EditorApp/EditorWorldService.h"
#include "Twisted/Gameing/Components/CCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>

#include <ImGuizmo.h>
#include <imgui.h>
#include <Twisted/Gameing/Components/CTransform.h>


namespace Twisted::Editor
{
	WorldViewPanel::WorldViewPanel() :EditorPanel("World View")
	{
		this->PanelResizeEvent.AddListener([this]() {
			auto fb = EditorWorldService::GetInstance()->renderer.EditorFrameBuffer;
			if (fb)
				FrameBuffer_GL::SetSize(*fb, Size);
			}
		);

	}

	void WorldViewPanel::PaintContent()
	{
		auto fb = EditorWorldService::GetInstance()->renderer.EditorFrameBuffer;
		if (fb && fb->Tex)
			ImGui::Image(
				(void*)(intptr_t)fb->Tex->TexID,
				ImVec2((float)Size.x,(float) Size.y),
				ImVec2(0, 1),  // top-left UV
				ImVec2(1, 0)   // bottom-right UV (flipped vertically)
			);

		DrawViewportGizmo(m_editorWorld->camEnt);
	}

	void WorldViewPanel::DrawViewportGizmo(Entity camEntity)
	{
		ImGuizmo::SetOrthographic(false); // or true if you want
		ImGuizmo::SetDrawlist();

		// Size of the gizmo window
		const float gizmoSize = 128.0f;

		auto [t, c] = camEntity.GetWorld()->GetComponents<TransformComponent, CameraComponent>(camEntity.GetID());

		Vec3f gizmoPos = t.LocalPos + t.GetForward() * 3.0f;
		Quat gizmoRot = t.LocalRot;
		Vec3f gizmoScale{ 1,1,1 };

		ImGuizmo::SetRect(ImGui::GetIO().DisplaySize.x - gizmoSize - 10, 10, gizmoSize, gizmoSize);






		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(gizmoPos.x, gizmoPos.y, gizmoPos.z));
		model *= glm::mat4_cast(glm::quat(gizmoRot.w, gizmoRot.x, gizmoRot.y, gizmoRot.z));
		model = glm::scale(model, glm::vec3(gizmoScale.x, gizmoScale.y, gizmoScale.z));

		Mat4x4f projMat= glm::ortho(c.LeftEdge, c.RightEdge, c.BotEdge, c.TopEdge,c.NearPlane,c.FarPlane);
		Mat4x4f camViewMat = glm::mat4_cast(glm::quat(t.LocalRot.w, t.LocalRot.x, t.LocalRot.y, t.LocalRot.z));
		auto InverseCamRotmatrix = t.GetInvertRotationMatrix();

		ImGuizmo::Manipulate(
			glm::value_ptr(camViewMat),   // identity view matrix for this widget
			glm::value_ptr(projMat),   // identity projection
			ImGuizmo::TRANSLATE,
			ImGuizmo::WORLD,
			glm::value_ptr(model)
		);


	}
}

REGISTER_EDITOR_PANEL(WorldViewPanel)

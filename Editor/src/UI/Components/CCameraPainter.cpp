#include "UI/Components/CCameraPainter.h"
#include "EditorApp/EditorRegistry.h"

#include "Components/CCamera.h"
#include "World.h"

#include "UI/ImguiExtensions.h"
#include <array>

namespace Twisted::Editor
{
	static constexpr auto kProjNames = magic_enum::enum_names<CameraProjectionType>();
	static constexpr int kProjCount = static_cast<int>(CameraProjectionType::COUNT);
	static const std::array<const char*, kProjCount> options = []() {
		std::array<const char*, kProjCount> arr{};
		for (std::size_t i = 0; i < kProjCount; ++i)
			arr[i] = kProjNames[i].data();
		return arr;
	}();

	void CCameraPainter::Paint(void* obj)
	{
		CameraComponent* camera = static_cast<CameraComponent*>(obj);

		bool isMainCamera = camera->IsMainCamera();
		if (Im::DrawToggle("Main camera", isMainCamera))
		{
			camera->CamManager->SetAsMainCamera((isMainCamera) ? camera : nullptr);
		}

		int currIndex = static_cast<int>(camera->ProjectionType);
		if (ImGui::Combo("Projection Type", &currIndex, options.data(), kProjCount))
		{
			camera->ProjectionType = static_cast<CameraProjectionType>(currIndex);
		}

		switch (camera->ProjectionType)
		{
		case(CameraProjectionType::PERSPECTIVE):
		{
			ImGui::DragFloat("FOV (deg)", &camera->FovDeg, 0.1f, 1.0f, 179.0f);
			ImGui::DragFloat("Aspect Ratio", &camera->AspectRatio, 0.01f, 0.01f, 10.0f);
			ImGui::DragFloat("Near Plane", &camera->NearPlane, 0.001f, 0.001f, camera->FarPlane);
			ImGui::DragFloat("Far Plane", &camera->FarPlane, 0.1f, camera->NearPlane, 10000.0f);
			break;
		}
		case(CameraProjectionType::ORTHOGRAPHIC):
		{
			ImGui::DragFloat("Left", &camera->LeftEdge, 0.01f);
			ImGui::DragFloat("Right", &camera->RightEdge, 0.01f);
			ImGui::DragFloat("Bottom", &camera->BotEdge, 0.01f);
			ImGui::DragFloat("Top", &camera->TopEdge, 0.01f);
			break;
		}
		default:
			return;
		}

		ImGui::ColorEdit4("Clear Color", (float*)&camera->clearParams.clearColor);

	}
}

REGISTER_COMPONENT_PAINTER(CCameraPainter, CameraComponent)


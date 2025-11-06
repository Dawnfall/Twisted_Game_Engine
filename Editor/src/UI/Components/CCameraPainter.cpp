#include "CCameraPainter.h"
#include "EditorRegistry.h"

#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Gameing/World.h"

#include <imgui.h>

namespace Twisted::Editor
{
	void CCameraPainter::Paint(void* obj)
	{
		CCamera* camera = static_cast<CCamera*>(obj);

        std::vector<const char*> options;
        for (int i = 0; i < static_cast<int>(CameraProjectionType::COUNT); ++i)
        {
            CameraProjectionType type = static_cast<CameraProjectionType>(i);
            options.emplace_back(ProjTypeToString(type));
        }

        int currIndex = static_cast<int>(camera->GetProjectionType());
        if (ImGui::Combo("Projection Type", &currIndex, options.data(), static_cast<int>(options.size())))
        {
            // Convert selected string back to enum
            camera->SetProjectionType(ProjTypeFromString(options[currIndex]));
        }

        
        switch (camera->GetProjectionType())
        {
        case(CameraProjectionType::PERSPECTIVE):
        {
            float fov = camera->GetFovInDeg();
            if (ImGui::DragFloat("FOV (deg)", &fov, 0.1f, 1.0f, 179.0f))
                camera->SetFovInDeg(fov);

            float aspect = camera->GetAspectRatio();
            if (ImGui::DragFloat("Aspect Ratio", &aspect, 0.01f, 0.01f, 10.0f))
                camera->SetAspectRatio(aspect);

            float nearP = camera->GetNearPlane();
            if (ImGui::DragFloat("Near Plane", &nearP, 0.001f, 0.001f, camera->GetFarPlane()))
                camera->SetNearPlane(nearP);

            float farP = camera->GetFarPlane();
            if (ImGui::DragFloat("Far Plane", &farP, 0.1f, camera->GetNearPlane(), 10000.0f))
                camera->SetFarPlane(farP);
            break;
        }
        case(CameraProjectionType::ORTHOGRAPHIC):
        {
            float left = camera->GetLeftEdge();
            if (ImGui::DragFloat("Left", &left, 0.01f))
                camera->SetLeftEdge(left);

            float right = camera->GetRightEdge();
            if (ImGui::DragFloat("Right", &right, 0.01f))
                camera->SetRightEdge(right);

            float bottom = camera->GetBotEdge();
            if (ImGui::DragFloat("Bottom", &bottom, 0.01f))
                camera->SetBotEdge(bottom);

            float top = camera->GetTopEdge();
            if (ImGui::DragFloat("Top", &top, 0.01f))
                camera->SetTopEdge(top);
            break;
        }
        default:
            return;
        }


	}
}

REGISTER_COMPONENT_PAINTER(CCameraPainter,CCamera)

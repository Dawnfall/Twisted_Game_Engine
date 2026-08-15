#include "UI/Components/ColliderPainter.h"
#include "EditorApp/EditorRegistry.h"
#include "Colliders/ColliderComponent.h"
#include "UI/ImguiExtensions.h"

namespace Twisted::Editor
{
    static const char* kShapeNames[]  = { "Box", "Sphere", "Capsule" };
    static const char* kMotionNames[] = { "Static", "Dynamic", "Kinematic" };

    void ColliderPainter::Paint(void* obj)
    {
        ColliderComponent* comp = static_cast<ColliderComponent*>(obj);

        int shapeIdx = static_cast<int>(comp->shape);
        if (ImGui::Combo("Shape", &shapeIdx, kShapeNames, 3))
            comp->shape = static_cast<ColliderShape>(shapeIdx);

        int motionIdx = static_cast<int>(comp->motionType);
        if (ImGui::Combo("Motion Type", &motionIdx, kMotionNames, 3))
            comp->motionType = static_cast<MotionType>(motionIdx);

        switch (comp->shape)
        {
        case ColliderShape::Box:
            Im::Vec3DragField("Half Extents", &comp->halfExtents.x, 0.01f);
            break;
        case ColliderShape::Sphere:
            ImGui::DragFloat("Radius", &comp->radius, 0.01f, 0.001f);
            break;
        case ColliderShape::Capsule:
            ImGui::DragFloat("Radius", &comp->radius, 0.01f, 0.001f);
            ImGui::DragFloat("Half Height", &comp->capsuleHalfHeight, 0.01f, 0.001f);
            break;
        }
    }
}

REGISTER_COMPONENT_PAINTER(ColliderPainter, ColliderComponent)

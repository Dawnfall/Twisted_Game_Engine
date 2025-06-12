#pragma once
#include "DetailsRenderer.h"

#include "Twisted/Gameing/Components/CTransform.h"
#include "imgui.h"

namespace Twisted::Editor
{
	template<>
	class DetailsRenderer<CTransform>
	{
	public:
		void RenderDetails(CTransform* transform)
		{
			// 3. Editable position
			Vec3f position = transform->GetLocalPosition();
			if (ImGui::InputFloat3("Position", &position.x))
			{
				transform->SetLocalPosition(position);
			}

			// 4. Editable rotation (Euler angles in degrees)
			Vec3f rotationEuler = transform->GetLocalRotationEulerRad() * (180.0f / 3.14159265359f); // radians to degrees
			if (ImGui::InputFloat3("Rotation", &rotationEuler.x))
			{
				Vec3f rotationEulerRad = rotationEuler * (3.14159265359f / 180.0f); // degrees to radians
				transform->SetLocalRotation(Quat(rotationEulerRad));
			}

			// 5. Editable scale
			Vec3f scale = transform->GetLocalScale();
			if (ImGui::InputFloat3("Scale", &scale.x))
			{
				transform->SetLocalScale(scale);
			}
		}

		std::string GetName() { return "Transform"; }
	};
}
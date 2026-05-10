#include "SpotLightPainter.h"
#include "UI/ImguiExtensions.h"
#include "Components/SpotLightComponent.h"
namespace Twisted::Editor
{
	void SpotLightPainter::Paint(void* obj)
	{
		SpotLightComponent* spotLight = static_cast<SpotLightComponent*>(obj);

		ImGui::ColorEdit4("Color", &spotLight->color[0]);
		ImGui::InputFloat("Intensity", &spotLight->intensity);
		ImGui::InputFloat("Radius", &spotLight->radius);
		ImGui::InputFloat("Outer", &spotLight->innerAngleDeg);
		ImGui::InputFloat("Inner", &spotLight->outerAngleDeg);

	}
}


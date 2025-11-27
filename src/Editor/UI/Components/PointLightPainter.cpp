#include "PointLightPainter.h"

#include "UI/ImguiExtensions.h"
#include "Twisted/Gameing/Components/PointLightComponent.h"

namespace Twisted::Editor
{
	void PointLightPainter::Paint(void* obj)
	{
		PointLightComponent* pointLight = static_cast<PointLightComponent*>(obj);

		ImGui::ColorEdit4("Color", &pointLight->color[0]);
		ImGui::InputFloat("Intensity", &pointLight->intensity);
		ImGui::InputFloat("Radius", &pointLight->radius);
	}
}


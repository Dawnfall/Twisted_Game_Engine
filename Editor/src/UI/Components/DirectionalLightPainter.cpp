#include "DirectionalLightPainter.h"

#include "UI/ImguiExtensions.h"
#include "Components/DirectionalLightComponent.h"

namespace Twisted::Editor
{
	void DirectionalLightPainter::Paint(void* obj)
	{
		DirectionalLightComponent* dirLight = static_cast<DirectionalLightComponent*>(obj);

		ImGui::ColorEdit4("Color", &dirLight->color[0]);
		ImGui::InputFloat("Intensity", &dirLight->intensity);
	}
}

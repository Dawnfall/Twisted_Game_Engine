//#include "CLightPainter.h"
//#include "EditorRegistry.h"
//
//#include "UI/ImguiExtensions.h"
//
//#include <array>
//
//namespace Twisted::Editor
//{
//	static std::array<const char*,3> options
//	{
//		DIR_LIGHT_NAME,
//		POINT_LIGHT_NAME,
//		SPOT_LIGHT_NAME,
//	};
//
//	void CLightPainter::Paint(void* obj)
//	{
//		CLight* light = static_cast<CLight*>(obj);
//
//		int currIndex = static_cast<int>(light->lightType);
//		if (ImGui::Combo("Light Type", &currIndex, options.data(), static_cast<int>(options.size())))
//		{
//			light->lightType = StringToLightType(options[currIndex]);
//		}
//		switch (light->lightType)
//		{
//		case LightType::DIRECTIONAL:
//			ImGui::ColorEdit4("Color", &light->color[0]);
//			ImGui::InputFloat("Intensity", &light->intensity);
//			break;
//		case LightType::POINT:
//			ImGui::ColorEdit4("Color", &light->color[0]);
//			ImGui::InputFloat("Intensity", &light->intensity);
//			ImGui::InputFloat("Radius", &light->radius);
//			break;
//		case LightType::SPOT:
//			ImGui::ColorEdit4("Color", &light->color[0]);
//			ImGui::InputFloat("Intensity", &light->intensity);
//			ImGui::InputFloat("Radius", &light->radius);
//			ImGui::InputFloat("Outer", &light->innerAngleDeg);
//			ImGui::InputFloat("Inner", &light->outerAngleDeg);
//			break;
//		}
//	}
//}
//
//REGISTER_COMPONENT_PAINTER(CLightPainter, CLight)

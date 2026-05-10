#include "MaterialPainter.h"
#include "Material.h"

#include "UI/ImguiExtensions.h"
#include "Shader.h"
#include "Utils/WPtr.h"
#include "Texture.h"
#include "Utils/GlmUtils.h"
#include "EditorApp/EditorRegistry.h"
#include "AssetsService.h"
#include "Application/Application.h"
#include <imgui.h>

namespace Twisted::Editor
{
	void MaterialPainter::PaintUniforms(Material* material)
	{
		for (auto& uniVar : material->GetValues())
		{
			const char* label = uniVar.editorName.c_str();
			std::visit([&uniVar, label](auto& val) {
				using T = std::decay_t<decltype(val)>;

				if constexpr (std::is_same_v<T, bool>) {
					ImGui::Checkbox(label, &val);
				}
				else if constexpr (std::is_same_v<T, float>) {
					ImGui::DragFloat(label, &val, 0.01f);
				}
				else if constexpr (std::is_same_v<T, double>) {
					float tmp = static_cast<float>(val);
					if (ImGui::DragFloat(label, &tmp, 0.01f))
						val = tmp;
				}
				else if constexpr (std::is_same_v<T, int>) {
					ImGui::DragInt(label, &val);
				}
				else if constexpr (std::is_same_v<T, Vec2f>) {
					ImGui::DragFloat2(label, &val[0], 0.01f);
				}
				else if constexpr (std::is_same_v<T, Vec3f>) {
					ImGui::DragFloat3(label, &val[0], 0.01f);
				}
				else if constexpr (std::is_same_v<T, Vec4f>) {
					if (uniVar.editorType == ShaderPropertyType::Color)
						ImGui::ColorEdit4(label, &val[0]);
					else
						ImGui::DragFloat4(label, &val[0], 0.01f);
				}
				else if constexpr (std::is_same_v<T, TextureValue>) {
					Texture* tex    = val.tex.get();
					Texture* newTex = Im::ObjectDropField<Texture>(uniVar.editorName, tex);
					if (newTex != tex)
						val.tex = newTex;
				}
				}, uniVar.val);
		}
	}

	void MaterialPainter::Paint(void* obj)
	{
		Material* material = static_cast<Material*>(obj);
		if (!material)
			return;

		material->SetShader(Im::ObjectDropField<Shader>("Shader", material->GetShader()));
		PaintUniforms(material);

		if (ImGui::Button("Save"))
		{
			auto assetsLayer = Application::GetInstance().GetService<AssetsService>();
			assetsLayer->Save(assetsLayer->GetObjectUuid(material));
		}
	}
}

REGISTER_DETAILS_PAINTER(Material, MaterialPainter)


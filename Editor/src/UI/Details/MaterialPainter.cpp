#include "MaterialPainter.h"
#include "Twisted/Rendering/Material.h"

#include "UI/ImguiExtensions.h"
#include "Twisted/Rendering/Shader.h"
#include "Utils/WPtr.h"
#include "Twisted/Rendering/Texture.h"
#include "Utils/GlmUtils.h"
#include "Twisted/AssetsLayer/AssetInfo.h"
#include "EditorApp/EditorRegistry.h"
#include "Twisted/AssetsLayer/AssetsService.h"

#include <imgui.h>

namespace Twisted::Editor
{
	void MaterialPainter::Paint(void* obj)
	{
		Material* material = static_cast<Material*>(obj);
		if (!material)
			return;

		material->SetShader(Im::ObjectDropField<Shader>("Shader", material->GetShader()));

		if (!material)
			return;
		for (auto& uniVar : material->GetValues())
		{
			(void)uniVar;
			std::visit([&material, &uniVar](auto& val) {
				using T = std::decay_t<decltype(val)>;

				if constexpr (std::is_same_v<T, bool>) {
					ImGui::Checkbox(uniVar.name.data(), &val);
				}
				else if constexpr (std::is_same_v<T, float>) {
					ImGui::InputFloat(uniVar.name.data(), &val);
				}
				else if constexpr (std::is_same_v<T, int>) {
					ImGui::InputInt(uniVar.name.data(), &val);
				}
				else if constexpr (std::is_same_v<T, Vec3f>) {
					ImGui::InputFloat3(uniVar.name.data(), &val[0]);
				}
				else if constexpr (std::is_same_v<T, Vec4f>) {
					ImGui::InputFloat4(uniVar.name.data(), &val[0]);
				}
				else if constexpr (std::is_same_v<T, TextureValue>) {
					Texture* tex = val.tex.get();
					Twisted::Texture* newTexPtr = Im::ObjectDropField<Texture>(uniVar.name, tex);
					if (newTexPtr != tex)
						val.tex = newTexPtr;
				}
				else {
					TWISTED_ERROR("Unsupported material uniform type to assign to shader!");
				}
				}, uniVar.val);

		}

		if (ImGui::Button("Save"))
		{
			auto assetsLayer = AssetsService::GetInstance();
			AssetInfo* asset = assetsLayer->GetObjectAssetInfo(material);
			assetsLayer->SaveAssetManaged(dynamic_cast<FileAssetInfo*>(asset));
		}
	}
}

REGISTER_DETAILS_PAINTER(Material, MaterialPainter)


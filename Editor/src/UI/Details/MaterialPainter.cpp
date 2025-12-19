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

		//Twisted::Shader* newPtr = Im::ObjectDropField<Shader>("Shader", material->Shader);

		if (material->Shader)
			for (const auto& uniform : material->Shader->Uniforms)
			{
				switch (uniform.Type)
				{
				case ShaderVarType::SAMPLER2D:
				{
					auto res = material->Get<WPtr<Texture>>(uniform.Name);
					Texture* tex = res.has_value() ? res.value().get() : nullptr;

					Twisted::Texture* newTexPtr = Im::ObjectDropField<Texture>(uniform.Name, tex);
					if (newTexPtr != tex)
						material->Set<WPtr<Texture>>(uniform.Name, WPtr<Texture>(newTexPtr));
					break;
				}
				case ShaderVarType::VEC4_F:
				{
					auto res = material->Get<Vec4f>(uniform.Name);

					Vec4f vec = res.has_value() ? res.value() : Vec4f{};
					if (ImGui::InputFloat4(uniform.Name.c_str(), &vec.x))
						material->Set<Vec4f>(uniform.Name, vec);
					break;
				}
				}
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


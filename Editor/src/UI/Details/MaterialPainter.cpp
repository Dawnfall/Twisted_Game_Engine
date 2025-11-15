#include "MaterialPainter.h"
#include "Twisted/Rendering/Material.h"

#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"
#include <Twisted/Rendering/Shader.h>
#include <Utils/WPtr.h>
#include <Twisted/Rendering/Texture.h>
#include <Utils/GlmUtils.h>

namespace Twisted::Editor
{
	void MaterialPainter::Paint(void* obj)
	{
		Material* material = static_cast<Material*>(obj);
		if (!material)
			return;

		Shader* shader = material->GetShader();
		auto newPtr = Im::ObjectDropField<Shader>("Shader", shader);
		material->SetShader(newPtr);

		if (shader)
			for (const auto& uniform : shader->Uniforms)
			{
				switch (uniform.Type)
				{
				case ShaderVarType::SAMPLER2D:
				{
					auto res = material->Get<WPtr<Texture>>(uniform.Name);
					Texture* tex = res.has_value() ? res.value().get() : nullptr;

					auto newPtr = Im::ObjectDropField<Texture>(uniform.Name, tex);
					if (newPtr != tex)
						material->Set<WPtr<Texture>>(uniform.Name, WPtr<Texture>(newPtr));
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
			auto assetsLayer = AssetsLayer::GetInstance();
			AssetInfo* asset = assetsLayer->GetObjectAssetInfo(material);
			assetsLayer->SaveAssetManaged(dynamic_cast<FileAssetInfo*>(asset));
		}
	}
}

REGISTER_DETAILS_PAINTER(Material, MaterialPainter)


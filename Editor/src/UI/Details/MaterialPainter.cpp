#include "MaterialPainter.h"
#include "Twisted/Rendering/Material.h"

#include "UI/ImguiExtensions.h"
#include "UI/ComponentPainter.h"

namespace Twisted::Editor
{
	void MaterialPainter::Paint(void* obj)
	{
		Material* material = static_cast<Material*>(obj);
		if (!material)
			return;

		auto newPtr = Im::ObjectDropField<Shader>("Shader", material->GetShader());
		material->SetShader(newPtr);

		if (ImGui::Button("Save"))
		{
			auto& assetsLayer = AssetsLayer::GetInstance();
			AssetUuid asset=assetsLayer.GetObjectAsset(material);
			assetsLayer.SaveAsset(asset);
		}
		if (!material->GetShader())
			return;

		//TODO: render uniforms
	}
}

REGISTER_DETAILS_PAINTER(Material,MaterialPainter)

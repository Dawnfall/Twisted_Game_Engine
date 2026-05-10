#include "CRendererPainter.h"
#include "EditorApp/EditorRegistry.h"
#include "UI/Details/MaterialPainter.h"
#include <imgui.h>

namespace Twisted::Editor
{
	void CRendererPainter::Paint(void* obj)
	{
		RendererComponent* renderer = static_cast<RendererComponent*>(obj);

		renderer->material = Im::ObjectDropField<Material>("Material", renderer->material);

		if (renderer->material)
		{
			ImGui::Indent();
			if (ImGui::CollapsingHeader("Material Properties"))
				MaterialPainter::PaintUniforms(renderer->material);
			ImGui::Unindent();
		}

		renderer->mesh = Im::ObjectDropField<Mesh>("Mesh", renderer->mesh);
	}
}

REGISTER_COMPONENT_PAINTER(CRendererPainter, RendererComponent)

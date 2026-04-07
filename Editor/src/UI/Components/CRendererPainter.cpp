#include "CRendererPainter.h"
#include "EditorApp/EditorRegistry.h"

namespace Twisted::Editor
{
	void CRendererPainter::Paint(void* obj)
	{
		RendererComponent* renderer = static_cast<RendererComponent*>(obj);

		renderer->material = Im::ObjectDropField<Material>("Material", renderer->material);
		renderer->mesh = Im::ObjectDropField<Mesh>("Mesh", renderer->mesh);
	}
}

REGISTER_COMPONENT_PAINTER(CRendererPainter, RendererComponent)

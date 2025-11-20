#include "CRendererPainter.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	void CRendererPainter::Paint(void* obj)
	{
		CRenderer* renderer = static_cast<CRenderer*>(obj);

		Im::ObjectDropField<Material>("Material", renderer->material);
		Im::ObjectDropField<Mesh>("Mesh", renderer->mesh);
	}
}

REGISTER_COMPONENT_PAINTER(CRendererPainter, CRenderer)

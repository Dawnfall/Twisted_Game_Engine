#include "CRendererPainter.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	void CRendererPainter::Paint(void* obj)
	{
		CRenderer* renderer = static_cast<CRenderer*>(obj);

		auto newMaterial = Im::ObjectDropField<Material>("Material", renderer->GetSharedMaterial());
		renderer->SetSharedMaterial(newMaterial);
		auto newMesh = Im::ObjectDropField<Mesh>("Mesh", renderer->GetSharedMesh());
		renderer->SetSharedMesh(newMesh);
	}
}

REGISTER_COMPONENT_PAINTER(CRendererPainter, CRenderer)

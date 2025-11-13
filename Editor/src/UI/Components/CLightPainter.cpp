#include "CLightPainter.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	void CLightPainter::Paint(void* obj)
	{
		CLight* light = static_cast<CLight*>(obj);
	}
}

REGISTER_COMPONENT_PAINTER(CLightPainter, CLight)

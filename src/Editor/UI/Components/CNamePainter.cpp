#include "CNamePainter.h"
#include "EditorApp/EditorRegistry.h"

namespace Twisted::Editor
{
	void CNamePainter::Paint(void* obj)
	{
		NameComponent* name = static_cast<NameComponent*>(obj);

		Im::InputTextToken token;

		token.DoAutoFocus = false;
		token.PreLabel = "Name";
		token.Text = name->Name;

		if (Im::InputText(token))
			name->Name = token.Text;
	}
}

REGISTER_COMPONENT_PAINTER(CNamePainter, NameComponent)

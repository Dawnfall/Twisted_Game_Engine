#include "CNamePainter.h"
#include "EditorRegistry.h"

namespace Twisted::Editor
{
	void CNamePainter::Paint(void* obj)
	{
		CName* name = static_cast<CName*>(obj);

		Im::InputTextToken token;

		token.DoAutoFocus = false;
		token.PreLabel = "Name";
		token.Text = name->Name;

		if (Im::InputText(token))
			name->Name = token.Text;
	}
}

REGISTER_COMPONENT_PAINTER(CNamePainter, CName)

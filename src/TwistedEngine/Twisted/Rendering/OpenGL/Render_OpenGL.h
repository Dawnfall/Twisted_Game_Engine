#pragma once
#include "AppCore.h"

namespace Twisted::Render_GL
{
	void TWISTED_API Init();

	void TWISTED_API EnableDepthTest(bool doTest);

	void TWISTED_API EnableStencilTest(bool doTest);

}
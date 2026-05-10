#pragma once

#include "AppCore.h"
#include "Data/RenderContext.h"

namespace Twisted::Render
{
	void TWISTED_API Init();

	void TWISTED_API InitSurface(void* nativeWindowHandle);

	void TWISTED_API EnableDepthTest(bool doTest);

	void TWISTED_API EnableStencilTest(bool doTest);

	void TWISTED_API ForwardRender(const RenderContext& renderContext);
}
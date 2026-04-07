#pragma once

#include "AppCore.h"
#include "Twisted/Rendering/RenderConvert.h"



namespace Twisted::Render
{
	void TWISTED_API Init();

	void TWISTED_API EnableDepthTest(bool doTest);

	void TWISTED_API EnableStencilTest(bool doTest);

	void TWISTED_API ForwardRender(const RenderContext& renderContext);
}
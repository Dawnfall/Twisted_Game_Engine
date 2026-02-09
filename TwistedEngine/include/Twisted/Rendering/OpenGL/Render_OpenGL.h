#ifndef TWISTED_D3D

#pragma once
#include "AppCore.h"
#include "Mesh_OpenGL.h"
#include "Twisted/Rendering/Data/RenderContext.h"

namespace Twisted::GL
{
	void TWISTED_API Init();

	void TWISTED_API EnableDepthTest(bool doTest);

	void TWISTED_API EnableStencilTest(bool doTest);

	void TWISTED_API ForwardRender(const RenderContext& context);

	void TWISTED_API RenderMesh(const Mesh_OpenGL& mesh);

}

#endif
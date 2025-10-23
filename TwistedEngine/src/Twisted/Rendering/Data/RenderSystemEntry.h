#pragma once

#include "Twisted/Gameing/Components/CCamera.h"
#include "Twisted/Rendering/Mesh.h"
#include "Twisted/Rendering/Shader.h"
#include "Twisted/Rendering/FrameBuffer.h"

namespace Twisted
{
	struct RenderSystemEntry
	{
		Mesh* mesh;
		Material* material;
		FrameBuffer* framebuffer;
	};
}
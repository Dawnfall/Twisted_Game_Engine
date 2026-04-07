#ifndef TWISTED_D3D
#pragma once
#include "AppCore.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/Data/ClearParams.h"
#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"
#include <glad/glad.h>

namespace Twisted
{
	struct FramebufferBackend
	{
		GLuint Id = 0;
		bool IsValid() const { return Id != 0; }
	};
}

namespace Twisted::GL
{
	void TWISTED_API ClearBuffer(const ClearParams& clearParams); // assumes bound framebuffer
}

#endif

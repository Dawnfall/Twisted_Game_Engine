#ifdef TWISTED_BACKEND_OPENGL
#pragma once
#include "AppCore.h"

#include "FrameBuffer.h"
#include "Data/ClearParams.h"
#include "OpenGL/Texture_OpenGL.h"
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

#ifndef TWISTED_D3D
#pragma once
#include "AppCore.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/Data/ClearParams.h"
#include "Twisted/Rendering/OpenGL/Texture_OpenGL.h"
#include <Utils/GlmUtils.h>
#include "Utils/WPtr.h"

namespace Twisted
{
	struct FramebufferBackend
	{
		GLuint Id = 0;
	};
}

namespace Twisted::GL
{

	inline void BindFramebuffer(GLuint fbID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbID);
		//if (IsDirty)
		//	Update();
		//glViewport(0, 0, Size.x, Size.y);
	}

	inline void UnbindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void TWISTED_API Blit(FramebufferBackend& framebuffer, GLuint destID);
	void TWISTED_API ClearBuffer(const ClearParams& clearParams); //assumes bound buffer
}

#endif
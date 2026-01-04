#pragma once
#include "AppCore.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include "Twisted/Rendering/Data/ClearParams.h"
#include <Utils/GlmUtils.h>

namespace Twisted::FrameBuffer_GL
{
	void TWISTED_API Init(FrameBuffer& framebuffer);
	void TWISTED_API Destroy(FrameBuffer& framebuffer);

	void TWISTED_API Bind(FrameBuffer& frameBuffer);
	void TWISTED_API UnBind();
	void TWISTED_API Update(FrameBuffer& framebuffer);
	void TWISTED_API Blit(FrameBuffer& framebuffer, unsigned int destID);
	void TWISTED_API SetSize(FrameBuffer& framebuffer, const Vec2i& size);

	void TWISTED_API ClearBuffer(const ClearParams& clearParams); //assumes bound buffer
}
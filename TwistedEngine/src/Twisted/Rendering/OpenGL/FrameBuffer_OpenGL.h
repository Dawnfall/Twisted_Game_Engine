#pragma once
#include "AppCore.h"

#include "Twisted/Rendering/FrameBuffer.h"
#include <Utils/GlmUtils.h>

namespace Twisted::FrameBuffer_GL
{
	void Init(FrameBuffer& framebuffer);
	void Destroy(FrameBuffer& framebuffer);

	void Bind(FrameBuffer& frameBuffer);
	void UnBind();
	void Update(FrameBuffer& framebuffer);
	void Blit(FrameBuffer& framebuffer, unsigned int destID);
	void TWISTED_API SetSize(FrameBuffer& framebuffer, const Vec2i& size);

	void ClearBuffer(const ClearParams& clearParams); //assumes bound buffer
}
#pragma once
#include "FrameBuffer.h"

namespace Twisted
{
	enum class RenderTargetType
	{
		NONE,
		FRAMEBUFFER
	};

	class RenderTarget
	{
	public:

	private:
		FrameBuffer* m_buffer;
	};
}


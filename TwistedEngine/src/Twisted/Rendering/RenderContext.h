#pragma once
#include "AppCore.h"
#include "FrameBuffer.h"
#include "Twisted/Data/Color.h"

namespace Twisted::Render
{
	class TWISTED_API RenderContext
	{
	public:
		RenderContext(int w, int h);

		void Bind();
		void UnBind();

		void SetEnableDepthTest(bool doEnable)
		{
			doDepthTest = doEnable;
		}
		void SetEnableDepthWrite(bool doEnable)
		{
			doDepthWrite = doEnable;
		}
		void SetEnableStencilTest(bool doEnable)
		{
			doStencilTest = doEnable;
		}

		void Resize(int width, int height);
		void Clear(Color color)const;

		bool doDepthTest = true;
		bool doStencilTest = true;
		bool doDepthWrite = true;

		URef<FrameBuffer> frameBuffer;
	};
}
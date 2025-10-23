#pragma once
#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Twisted/Data/Color.h"

#include <string>

namespace Twisted
{
	class TWISTED_API GraphicsContext
	{
	public:

		GraphicsContext(void* windowHandle);

		~GraphicsContext();

		void SwapBuffers();

		void SetVSync(int deltaFrames);

		void Clear(const Color& color);

		void* GetRawPointer() { return m_windowHandle; }

		void* GetContextAddress() { return m_hdc; }

		Vec2i GetSize() const;

		Vec2i GetPosition() const;

		std::string GetTitle() const;

		void SetTitle(const std::string& newTitle);

		void SetSize(Vec2i newSize);

		void SetPosition(Vec2i newPosition);

		void SetFullscreen();

		void SetWindowed(Vec2i size, Vec2i pos);

	private:
		void* m_windowHandle;
		void* m_hdc;
		void* m_glrc;
	};
}
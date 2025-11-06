#pragma once
#include "AppCore.h"
#include "Utils/GlmUtils.h"
#include "Data/Color.h"

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


		Vec2i GetSize() const;

		Vec2i GetPosition() const;

		std::string GetTitle() const;

		void SetTitle(const std::string& newTitle);

		void SetSize(Vec2i newSize);

		void SetPosition(Vec2i newPosition);

		void SetWindowed(Vec2i size, Vec2i pos);

		void* GetContextAddress() { return m_hdc; }

	private:
		void* m_windowHandle=nullptr;
		void* m_hdc = nullptr;
		void* m_glrc = nullptr;
	};
}
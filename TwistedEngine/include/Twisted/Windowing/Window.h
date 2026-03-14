#pragma once
#include "AppCore.h"
#include "Utils/Event.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Input.h"

#include <memory>

namespace Twisted
{
	struct WindowBackend;
	class TWISTED_API Window
	{
	public:
		Window(const std::string& title, Vec2i size, Vec2i position);
		Window(const Window& other) = delete;
		Window(Window&& other) = default;
		~Window();

		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = default;

		Vec2i GetSize()const;
		Vec2i GetClientSize() const;
		Vec2i GetPosition()const;
		std::string GetTitle()const;

		void SetTitle(const std::string& newName);
		void SetSize(Vec2i newSize);
		void SetPosition(Vec2i newPosition);

		void Maximize();
		void SetFullScreen(bool isFullScreen); //TODO:... for now only on mainMonitor
		void SetWindowed(Vec2i size, Vec2i pos);

		void* GetRawPointer();

		bool CloseWindow();
		void Clear(const Color& color);
		void SetVSync(int deltaFrames);

		void SwapBuffers();

		Event<> CloseWindowEvent;
		Event<Vec2i> WindowResizeEvent;

		const WindowBackend* GetBackend()const { return m_backend; }

	private:

		WindowBackend* m_backend = nullptr;
	
	};
}



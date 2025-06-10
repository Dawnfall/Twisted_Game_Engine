#pragma once
#include "AppCore.h"
#include "Data/Event.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"

#include <Windows.h>

namespace Twisted
{
	class TWISTED_API Window
	{
	public:
		Window(const std::string& title, Vec2i size, Vec2i position);
		Window(const Window& other) = delete;
		Window(Window&& other) = default;
		~Window();

		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = default;

		HWND GetWindowHandle();
		void* GetRawPointer();
		void* GetContextAdress();

		Vec2i GetSize()const;
		Vec2i GetPosition()const;
		std::string GetTitle()const;

		void SetTitle(const std::string& newName);
		void SetSize(Vec2i newSize);
		void SetPosition(Vec2i newPosition);
		void SetFullscreen(); //TODO:... for now only on mainMonitor
		void SetWindowed(Vec2i size, Vec2i pos);
		void SetVsync(int deltaFrames);

		void Clear(const Color& color);
		void SwapBuffers();
		void PollEvents();

		bool CloseWindow();

		Event<> CloseWindowEvent;
		Event<> WindowResizeEvent;
	
		static URef<Window> CreateNewWindow(const std::string& title, Vec2i size, Vec2i position);
	private:

		class Impl;
		Impl* m_impl;
	};
}

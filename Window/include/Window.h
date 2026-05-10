#pragma once
#include "AppCore.h"
#include "Utils/Event.h"
#include "Data/Color.h"
#include "Utils/GlmUtils.h"
#include "Input.h"

#include <memory>
#include <vector>
#include <filesystem>

namespace Twisted
{
	class WindowsService;
	struct WindowBackend;
	class TWISTED_API Window
	{
	public:
		Window(WindowsService* service,const std::string& title, Vec2i size, Vec2i position);
		Window(const Window& other) = delete;
		Window(Window&& other);
		~Window();

		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other);

		Vec2i GetSize()const;
		Vec2i GetClientSize() const;
		Vec2i GetPosition()const;
		std::string GetTitle()const;
		bool IsWindowMaximized()const;

		struct RestoreBounds { Vec2i pos; Vec2i size; };
		RestoreBounds GetRestoreBounds()const; // pre-maximize position and size

		void SetTitle(const std::string& newName);
		void SetSize(Vec2i newSize);
		void SetPosition(Vec2i newPosition);

		void Minimize();
		void Maximize();
		void Restore();
		void SetFullScreen(bool isFullScreen); //TODO:... for now only on mainMonitor
		void SetWindowed(Vec2i size, Vec2i pos);

		void SetTitleBarHeight(int height) { m_titleBarHeight = height; }
		int  GetTitleBarHeight() const { return m_titleBarHeight; }

		void* GetRawPointer();

		bool CloseWindow();
		void Clear(const Color& color);
		void SetVSync(int deltaFrames);

		void SwapBuffers();
		void AcquireGLContext();
		void ReleaseGLContext();

		Event<> CloseWindowEvent;
		Event<Vec2i> WindowResizeEvent;
		Event<std::vector<std::filesystem::path>> FilesDroppedEvent;

		const WindowBackend* GetBackend()const { return m_backend ? m_backend.get() : nullptr; }
		WindowsService* GetWindowsService() { return m_windowsService; }

	private:

		std::unique_ptr<WindowBackend> m_backend;
		WindowsService* m_windowsService = nullptr;
		int m_titleBarHeight = 0;
	};
}



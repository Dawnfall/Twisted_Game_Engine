#pragma once
#include "AppCore.h"
#include "Utils/Event.h"
#include "Utils/GlmUtils.h"
#include "Input.h"

#include <vector>
#include <filesystem>

struct GLFWwindow;

namespace Twisted
{
	class WindowsService;
	class TWISTED_API Window
	{
	public:
		Window(WindowsService* service, const std::string& title, Vec2i size, Vec2i position);
		Window(const Window& other) = delete;
		Window(Window&& other) noexcept;
		~Window();

		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept;

		Vec2i GetSize() const;
		Vec2i GetClientSize() const;
		Vec2i GetPosition() const;
		std::string GetTitle() const;
		bool IsWindowMaximized() const;

		struct RestoreBounds { Vec2i pos; Vec2i size; };
		RestoreBounds GetRestoreBounds() const;

		void SetTitle(const std::string& newName);
		void SetSize(Vec2i newSize);
		void SetPosition(Vec2i newPosition);

		void Minimize();
		void Maximize();
		void Restore();
		void SetFullScreen(bool isFullScreen);
		void SetWindowed(Vec2i size, Vec2i pos);

		void SetTitleBarHeight(int height) { m_titleBarHeight = height; }
		int  GetTitleBarHeight() const { return m_titleBarHeight; }

		void* GetRawPointer();

		bool CloseWindow();

		Event<> CloseWindowEvent;
		Event<Vec2i> WindowResizeEvent;
		Event<std::vector<std::filesystem::path>> FilesDroppedEvent;

		WindowsService* GetWindowsService() { return m_windowsService; }

	private:
		GLFWwindow*     m_glfwWindow     = nullptr;
		WindowsService* m_windowsService = nullptr;
		int             m_titleBarHeight = 0;
	};
}



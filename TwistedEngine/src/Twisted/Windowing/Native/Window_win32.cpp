#include "AppCore.h"
#ifndef GLFW_INCLUDE_NONE

#include "Twisted/Windowing/Window.h"
#include "Debug/Logger.h"

#include <Windows.h>
#include <windowsx.h>
[[nodiscard]] inline Twisted::MouseButton win32ToTwistedButton(UINT msg) noexcept
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		return Twisted::MouseButton::Left;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		return Twisted::MouseButton::Right;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		return Twisted::MouseButton::Middle;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	{
		// Determine if it’s XBUTTON1 or XBUTTON2
		// GET_XBUTTON_WPARAM macro extracts which X button
		UINT button = GET_XBUTTON_WPARAM(msg);
		if (button == XBUTTON1) return Twisted::MouseButton::Button4;
		if (button == XBUTTON2) return Twisted::MouseButton::Button5;
		return Twisted::MouseButton::Invalid;
	}
	default:
		return Twisted::MouseButton::Invalid;
	}
}

[[nodiscard]] inline Twisted::Key win32ToTwistedKey(WPARAM vk) noexcept
{
	switch (vk)
	{
	case VK_SPACE: return Twisted::Key::Space;
	case VK_OEM_7: return Twisted::Key::Apostrophe;
	case VK_OEM_COMMA: return Twisted::Key::Comma;
	case VK_OEM_MINUS: return Twisted::Key::Minus;
	case VK_OEM_PERIOD: return Twisted::Key::Period;
	case VK_OEM_2: return Twisted::Key::Slash;
	case '0': return Twisted::Key::D0;
	case '1': return Twisted::Key::D1;
	case '2': return Twisted::Key::D2;
	case '3': return Twisted::Key::D3;
	case '4': return Twisted::Key::D4;
	case '5': return Twisted::Key::D5;
	case '6': return Twisted::Key::D6;
	case '7': return Twisted::Key::D7;
	case '8': return Twisted::Key::D8;
	case '9': return Twisted::Key::D9;
	case VK_OEM_1: return Twisted::Key::Semicolon;
	case VK_OEM_PLUS: return Twisted::Key::Equal; // normal +/= key
	case 'A': return Twisted::Key::A;
	case 'B': return Twisted::Key::B;
	case 'C': return Twisted::Key::C;
	case 'D': return Twisted::Key::D;
	case 'E': return Twisted::Key::E;
	case 'F': return Twisted::Key::F;
	case 'G': return Twisted::Key::G;
	case 'H': return Twisted::Key::H;
	case 'I': return Twisted::Key::I;
	case 'J': return Twisted::Key::J;
	case 'K': return Twisted::Key::K;
	case 'L': return Twisted::Key::L;
	case 'M': return Twisted::Key::M;
	case 'N': return Twisted::Key::N;
	case 'O': return Twisted::Key::O;
	case 'P': return Twisted::Key::P;
	case 'Q': return Twisted::Key::Q;
	case 'R': return Twisted::Key::R;
	case 'S': return Twisted::Key::S;
	case 'T': return Twisted::Key::T;
	case 'U': return Twisted::Key::U;
	case 'V': return Twisted::Key::V;
	case 'W': return Twisted::Key::W;
	case 'X': return Twisted::Key::X;
	case 'Y': return Twisted::Key::Y;
	case 'Z': return Twisted::Key::Z;
	case VK_OEM_4: return Twisted::Key::LeftBracket;
	case VK_OEM_5: return Twisted::Key::Backslash;
	case VK_OEM_6: return Twisted::Key::RightBracket;
	case VK_OEM_3: return Twisted::Key::GraveAccent;
	case VK_ESCAPE: return Twisted::Key::Escape;
	case VK_RETURN: return Twisted::Key::Enter;
	case VK_TAB: return Twisted::Key::Tab;
	case VK_BACK: return Twisted::Key::Backspace;
	case VK_INSERT: return Twisted::Key::Insert;
	case VK_DELETE: return Twisted::Key::Delete;
	case VK_RIGHT: return Twisted::Key::Right;
	case VK_LEFT: return Twisted::Key::Left;
	case VK_DOWN: return Twisted::Key::Down;
	case VK_UP: return Twisted::Key::Up;
	case VK_PRIOR: return Twisted::Key::PageUp;
	case VK_NEXT: return Twisted::Key::PageDown;
	case VK_HOME: return Twisted::Key::Home;
	case VK_END: return Twisted::Key::End;
	case VK_CAPITAL: return Twisted::Key::CapsLock;
	case VK_SCROLL: return Twisted::Key::ScrollLock;
	case VK_NUMLOCK: return Twisted::Key::NumLock;
	case VK_SNAPSHOT: return Twisted::Key::PrintScreen;
	case VK_PAUSE: return Twisted::Key::Pause;
	case VK_F1: return Twisted::Key::F1;
	case VK_F2: return Twisted::Key::F2;
	case VK_F3: return Twisted::Key::F3;
	case VK_F4: return Twisted::Key::F4;
	case VK_F5: return Twisted::Key::F5;
	case VK_F6: return Twisted::Key::F6;
	case VK_F7: return Twisted::Key::F7;
	case VK_F8: return Twisted::Key::F8;
	case VK_F9: return Twisted::Key::F9;
	case VK_F10: return Twisted::Key::F10;
	case VK_F11: return Twisted::Key::F11;
	case VK_F12: return Twisted::Key::F12;
	case VK_F13: return Twisted::Key::F13;
	case VK_F14: return Twisted::Key::F14;
	case VK_F15: return Twisted::Key::F15;
	case VK_F16: return Twisted::Key::F16;
	case VK_F17: return Twisted::Key::F17;
	case VK_F18: return Twisted::Key::F18;
	case VK_F19: return Twisted::Key::F19;
	case VK_F20: return Twisted::Key::F20;
	case VK_F21: return Twisted::Key::F21;
	case VK_F22: return Twisted::Key::F22;
	case VK_F23: return Twisted::Key::F23;
	case VK_F24: return Twisted::Key::F24;

		// Numpad
	case VK_NUMPAD0: return Twisted::Key::KP_0;
	case VK_NUMPAD1: return Twisted::Key::KP_1;
	case VK_NUMPAD2: return Twisted::Key::KP_2;
	case VK_NUMPAD3: return Twisted::Key::KP_3;
	case VK_NUMPAD4: return Twisted::Key::KP_4;
	case VK_NUMPAD5: return Twisted::Key::KP_5;
	case VK_NUMPAD6: return Twisted::Key::KP_6;
	case VK_NUMPAD7: return Twisted::Key::KP_7;
	case VK_NUMPAD8: return Twisted::Key::KP_8;
	case VK_NUMPAD9: return Twisted::Key::KP_9;
	case VK_DECIMAL: return Twisted::Key::KP_Decimal;
	case VK_DIVIDE: return Twisted::Key::KP_Divide;
	case VK_MULTIPLY: return Twisted::Key::KP_Multiply;
	case VK_SUBTRACT: return Twisted::Key::KP_Subtract;
	case VK_ADD: return Twisted::Key::KP_Add;
	case VK_SEPARATOR: return Twisted::Key::KP_Enter;

		// Modifiers
	case VK_LSHIFT: return Twisted::Key::LeftShift;
	case VK_LCONTROL: return Twisted::Key::LeftControl;
	case VK_LMENU: return Twisted::Key::LeftAlt;
	case VK_LWIN: return Twisted::Key::LeftSuper;
	case VK_RSHIFT: return Twisted::Key::RightShift;
	case VK_RCONTROL: return Twisted::Key::RightControl;
	case VK_RMENU: return Twisted::Key::RightAlt;
	case VK_RWIN: return Twisted::Key::RightSuper;
	case VK_APPS: return Twisted::Key::Menu;

	default: return Twisted::Key::Invalid;
	}
}

namespace Twisted
{
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		switch (uMsg)
		{
		case WM_CLOSE:
		{
			window->CloseWindowEvent.Invoke();
			break;
		}
		case WM_SIZE: //resize event
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			window->WindowResizeEvent.Invoke(Vec2i{ width,height });
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			Twisted::Key key = win32ToTwistedKey(wParam);
			Input::GetInstance().UpdateKey(key, true);
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			Twisted::Key key = win32ToTwistedKey(wParam);
			Input::GetInstance().UpdateKey(key, false);
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		{
			if (window)
			{
				Twisted::MouseButton button = win32ToTwistedButton(wParam);
				Input::GetInstance().UpdateMouseButton(button, true);
			}
			return 0;
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			Twisted::MouseButton button = win32ToTwistedButton(wParam);
			Input::GetInstance().UpdateMouseButton(button, false);
			break;
		}
		case WM_MOUSEMOVE:
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			Input::GetInstance().UpdateMousePosition(static_cast<float>(x), static_cast<float>(y));
			break;
		}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	static HWND createWindowHandle(const std::string& title, Vec2i size, Vec2i position)
	{
		// Register class
		WNDCLASS wc{};
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = "TwistedWin32Window";
		wc.style = CS_OWNDC;

		if (!RegisterClass(&wc))
		{
			MessageBox(nullptr, "Failed to register window class", "Error", MB_OK);
			return nullptr;
		}

		HWND hwnd = CreateWindowEx(
			0,
			wc.lpszClassName,
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			position.x, position.y, size.x, size.y,
			nullptr,
			nullptr,
			wc.hInstance,
			nullptr
		);

		return hwnd;
	}

	Window::Window(const std::string& title, Vec2i size, Vec2i position)
	{
		HWND hwnd = createWindowHandle(title, size, position);
		if (!hwnd)
		{
			TWISTED_ERROR("Failed to create Win32 window!");
			return;
		}

		m_pointer = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		m_context = std::make_unique<GraphicsContext>(hwnd);
		ShowWindow(hwnd, SW_SHOW);

		TWISTED_INFO("Win32 Window Created");
	}

	Window::~Window()
	{
		if (m_pointer)
			DestroyWindow(static_cast<HWND>(m_pointer));
	}

	Vec2i Window::GetSize() const
	{
		RECT rect{};
		if (m_pointer)
			GetClientRect(static_cast<HWND>(m_pointer), &rect);
		return Vec2i{ rect.right - rect.left, rect.bottom - rect.top };
	}

	Vec2i Window::GetPosition() const
	{
		RECT rect{};
		if (m_pointer)
			GetWindowRect(static_cast<HWND>(m_pointer), &rect);
		return Vec2i{ rect.left, rect.top };
	}

	std::string Window::GetTitle() const
	{
		char buffer[256]{};
		if (m_pointer)
			GetWindowTextA(static_cast<HWND>(m_pointer), buffer, 256);
		return std::string(buffer);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		if (m_pointer)
			SetWindowTextA(static_cast<HWND>(m_pointer), newTitle.c_str());
	}

	void Window::Maximize()
	{
		RECT work;
		MONITORINFO mi = { sizeof(mi) };
		if (GetMonitorInfo(MonitorFromWindow(static_cast<HWND>(m_pointer), MONITOR_DEFAULTTOPRIMARY), &mi))
			work = mi.rcWork;

		RECT clientRect = { 0, 0, work.right - work.left, work.bottom - work.top };

		// Adjust for window style (borders, title bar)
		AdjustWindowRectEx(&clientRect,
			GetWindowLong(static_cast<HWND>(m_pointer), GWL_STYLE),
			FALSE, // menu
			GetWindowLong(static_cast<HWND>(m_pointer), GWL_EXSTYLE));

		// clientRect now gives the total window size needed
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		// Compute top-left so that client area aligns with work.left/top
		int posX = work.left - clientRect.left;
		int posY = work.top - clientRect.top;

		SetWindowLong(static_cast<HWND>(m_pointer), GWL_STYLE, WS_VISIBLE); // or combine flags as needed
		SetWindowPos(static_cast<HWND>(m_pointer), HWND_TOP,
			posX, posY,
			width, height,
			SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	void Window::SetFullScreen(bool isFullScreen)
	{
		DWORD style = GetWindowLong(static_cast<HWND>(m_pointer), GWL_STYLE);
		if (isFullScreen)
		{
			// Remove border, title bar
			SetWindowLong(static_cast<HWND>(m_pointer), GWL_STYLE, style & ~(WS_OVERLAPPEDWINDOW));

			// Get monitor size
			MONITORINFO mi = { sizeof(mi) };
			if (GetMonitorInfo(MonitorFromWindow(static_cast<HWND>(m_pointer), MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowPos(static_cast<HWND>(m_pointer), HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}
		else
		{
			// Restore window style
			SetWindowLong(static_cast<HWND>(m_pointer), GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
			SetWindowPos(static_cast<HWND>(m_pointer), HWND_NOTOPMOST, 100, 100, 1280, 720,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}

	void Window::SetSize(Vec2i newSize)
	{
		if (m_pointer)
			SetWindowPos(static_cast<HWND>(m_pointer), nullptr, 0, 0, newSize.x, newSize.y, SWP_NOMOVE | SWP_NOZORDER);
	}

	void Window::SetPosition(Vec2i newPosition)
	{
		if (m_pointer)
			SetWindowPos(static_cast<HWND>(m_pointer), nullptr, newPosition.x, newPosition.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void Window::SetWindowed(Vec2i size, Vec2i pos)
	{
		SetSize(size);
		SetPosition(pos);
	}

	bool Window::CloseWindow()
	{
		if (m_pointer)
		{
			PostMessage(static_cast<HWND>(m_pointer), WM_CLOSE, 0, 0);
			TWISTED_INFO("Window closed");
			return true;
		}
		return false;
	}


	void* Window::GetRawPointer()
	{
		return m_pointer;
	}


	void Window::PollEvents() //make this static
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Window::PollMsgEvent.Invoke(&msg); //also this is static
		}
	}

}
#endif
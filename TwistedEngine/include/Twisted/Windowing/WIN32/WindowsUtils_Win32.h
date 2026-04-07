#pragma once

#include "Twisted/Windowing/Window.h"
#include "Twisted/Windowing/WindowsService.h"
#include "Twisted/Windowing/KeyCodes.h"
#include "Twisted/Windowing/ButtonCodes.h"
#include "Twisted/Windowing/WindowEvents.h"
#include <Windows.h>
#include <windowsx.h>

namespace Twisted
{
	inline const std::string WINDOW_CLASS_NAME = "TwistedWin32Window";

	inline static void PollEvents()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	inline static Twisted::KeyState win32KeyUpOrDown(UINT msg)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			return Twisted::KeyState::PRESSED;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			return Twisted::KeyState::RELEASED;

		default:
			TWISTED_WARN("Invalid key state detected: {}", msg);
			return Twisted::KeyState::INVALID;
		}
	}

	inline static Twisted::Key win32ToTwistedKey(WPARAM vk) noexcept
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

		default: 
			TWISTED_WARN("Invalid key detected: {}", vk);
			return Twisted::Key::Invalid;
		}
	}

	inline static std::pair<Twisted::MouseButton, Twisted::KeyState> win32ToTwistedButton(UINT msg) noexcept
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			return { Twisted::MouseButton::Left ,Twisted::KeyState::PRESSED };
		case WM_LBUTTONUP:
			return { Twisted::MouseButton::Left ,Twisted::KeyState::RELEASED };
		case WM_RBUTTONDOWN:
			return { Twisted::MouseButton::Right ,Twisted::KeyState::PRESSED };
		case WM_RBUTTONUP:
			return { Twisted::MouseButton::Right ,Twisted::KeyState::RELEASED };
		case WM_MBUTTONDOWN:
			return { Twisted::MouseButton::Middle ,Twisted::KeyState::PRESSED };
		case WM_MBUTTONUP:
			return { Twisted::MouseButton::Right ,Twisted::KeyState::RELEASED };
		case WM_XBUTTONDOWN:
		{
			// Determine if it’s XBUTTON1 or XBUTTON2
			// GET_XBUTTON_WPARAM macro extracts which X button
			UINT button = GET_XBUTTON_WPARAM(msg);
			if (button == XBUTTON1) return { Twisted::MouseButton::Button4 ,Twisted::KeyState::PRESSED };
			if (button == XBUTTON2) return { Twisted::MouseButton::Button5,Twisted::KeyState::PRESSED };
			return { Twisted::MouseButton::Invalid ,Twisted::KeyState::PRESSED };
		}
		case WM_XBUTTONUP:
		{
			// Determine if it’s XBUTTON1 or XBUTTON2
			// GET_XBUTTON_WPARAM macro extracts which X button
			UINT button = GET_XBUTTON_WPARAM(msg);
			if (button == XBUTTON1) return { Twisted::MouseButton::Button4 ,Twisted::KeyState::RELEASED };
			if (button == XBUTTON2) return { Twisted::MouseButton::Button5,Twisted::KeyState::RELEASED };
			return { Twisted::MouseButton::Invalid ,Twisted::KeyState::RELEASED };
		}
		default:
			TWISTED_WARN("Unknown mouse button detected: {}", msg);
			return { Twisted::MouseButton::Invalid ,Twisted::KeyState::INVALID };
		}
	}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hwnd, msg, wParam, lParam);

		WindowsService* service=window->GetWindowsService();

		//CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		//Window* window = static_cast<Window*>(cs->lpCreateParams);

		//TODO: use event registry to just trigger events from here

		if (msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
		{
			KeyEvent e;
			e.key = win32ToTwistedKey(wParam);
			e.state = win32KeyUpOrDown(msg);

			service->DispatchEvent(e);
			return 0;
		}

		if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
		{
			if (msg == WM_MOUSEMOVE)
			{
				MouseMoveEvent e;
				e.position = { static_cast<float>(GET_X_LPARAM(lParam)),
							   static_cast<float>(GET_Y_LPARAM(lParam)) };
				service->DispatchEvent(e);
				return 0;
			}

			if (msg == WM_MOUSEWHEEL)
			{
				MouseWheelEvent e;
				e.delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
				service->DispatchEvent(e);
				return 0;
			}

			MouseButtonEvent e;
			auto [button, state] = win32ToTwistedButton(msg);
			e.button = button;
			e.state = state;
			service->DispatchEvent(e);
			return 0;
		}
		switch (msg)
		{
		case WM_CLOSE:
		{
			WindowCloseEvent e;
			service->DispatchEvent(e);
			return 0;
		}
		case WM_SIZE:
		{
			WindowResizeEvent e;
			e.size.x = LOWORD(lParam);
			e.size.y = HIWORD(lParam);
			service->DispatchEvent(e);
		}
		//case WM_DESTROY:
		//{
		//	PostQuitMessage(0); // ends message loop
		//	return 0;
		//}
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

}
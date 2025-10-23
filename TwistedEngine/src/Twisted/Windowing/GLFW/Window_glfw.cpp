#include "AppCore.h"
#ifndef NATIVE_USE

#include "Twisted/Windowing/Window.h"
#include "Twisted/Data/Color.h"
#include "Twisted/Windowing/MonitorInfo.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> // Must be included after glfw3.h
#include "Debug/Logger.h"

[[nodiscard]] inline Twisted::MouseButton glfwToTwistedButton(int glfwButton) noexcept
{
	switch (glfwButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT:   return Twisted::MouseButton::Left;
	case GLFW_MOUSE_BUTTON_RIGHT:  return Twisted::MouseButton::Right;
	case GLFW_MOUSE_BUTTON_MIDDLE: return Twisted::MouseButton::Middle;
	case GLFW_MOUSE_BUTTON_4:      return Twisted::MouseButton::Button4;
	case GLFW_MOUSE_BUTTON_5:      return Twisted::MouseButton::Button5;
	case GLFW_MOUSE_BUTTON_6:      return Twisted::MouseButton::Button6;
	case GLFW_MOUSE_BUTTON_7:      return Twisted::MouseButton::Button7;
	case GLFW_MOUSE_BUTTON_8:      return Twisted::MouseButton::Button8;
	default:
	{
		TWISTED_WARN("Unsupporeted GLFW Button");
		return Twisted::MouseButton::Invalid; // fallback
	}
	}
}

[[nodiscard]] inline int twistedToGlfwButton(Twisted::MouseButton button) noexcept
{
	switch (button)
	{
	case Twisted::MouseButton::Left:   return GLFW_MOUSE_BUTTON_LEFT;
	case Twisted::MouseButton::Right:  return GLFW_MOUSE_BUTTON_RIGHT;
	case Twisted::MouseButton::Middle: return GLFW_MOUSE_BUTTON_MIDDLE;
	case Twisted::MouseButton::Button4:return GLFW_MOUSE_BUTTON_4;
	case Twisted::MouseButton::Button5:return GLFW_MOUSE_BUTTON_5;
	case Twisted::MouseButton::Button6:return GLFW_MOUSE_BUTTON_6;
	case Twisted::MouseButton::Button7:return GLFW_MOUSE_BUTTON_7;
	case Twisted::MouseButton::Button8:return GLFW_MOUSE_BUTTON_8;
	default:
	{
		TWISTED_WARN("Unsupported Twisted Button");
		return -1;
	}
	}
}

[[nodiscard]] inline Twisted::Key glfwToTwistedKey(int glfwKey) noexcept
{
	switch (glfwKey)
	{
	case GLFW_KEY_SPACE: return Twisted::Key::Space;
	case GLFW_KEY_APOSTROPHE: return Twisted::Key::Apostrophe;
	case GLFW_KEY_COMMA: return Twisted::Key::Comma;
	case GLFW_KEY_MINUS: return Twisted::Key::Minus;
	case GLFW_KEY_PERIOD: return Twisted::Key::Period;
	case GLFW_KEY_SLASH: return Twisted::Key::Slash;
	case GLFW_KEY_0: return Twisted::Key::D0;
	case GLFW_KEY_1: return Twisted::Key::D1;
	case GLFW_KEY_2: return Twisted::Key::D2;
	case GLFW_KEY_3: return Twisted::Key::D3;
	case GLFW_KEY_4: return Twisted::Key::D4;
	case GLFW_KEY_5: return Twisted::Key::D5;
	case GLFW_KEY_6: return Twisted::Key::D6;
	case GLFW_KEY_7: return Twisted::Key::D7;
	case GLFW_KEY_8: return Twisted::Key::D8;
	case GLFW_KEY_9: return Twisted::Key::D9;
	case GLFW_KEY_SEMICOLON: return Twisted::Key::Semicolon;
	case GLFW_KEY_EQUAL: return Twisted::Key::Equal;
	case GLFW_KEY_A: return Twisted::Key::A;
	case GLFW_KEY_B: return Twisted::Key::B;
	case GLFW_KEY_C: return Twisted::Key::C;
	case GLFW_KEY_D: return Twisted::Key::D;
	case GLFW_KEY_E: return Twisted::Key::E;
	case GLFW_KEY_F: return Twisted::Key::F;
	case GLFW_KEY_G: return Twisted::Key::G;
	case GLFW_KEY_H: return Twisted::Key::H;
	case GLFW_KEY_I: return Twisted::Key::I;
	case GLFW_KEY_J: return Twisted::Key::J;
	case GLFW_KEY_K: return Twisted::Key::K;
	case GLFW_KEY_L: return Twisted::Key::L;
	case GLFW_KEY_M: return Twisted::Key::M;
	case GLFW_KEY_N: return Twisted::Key::N;
	case GLFW_KEY_O: return Twisted::Key::O;
	case GLFW_KEY_P: return Twisted::Key::P;
	case GLFW_KEY_Q: return Twisted::Key::Q;
	case GLFW_KEY_R: return Twisted::Key::R;
	case GLFW_KEY_S: return Twisted::Key::S;
	case GLFW_KEY_T: return Twisted::Key::T;
	case GLFW_KEY_U: return Twisted::Key::U;
	case GLFW_KEY_V: return Twisted::Key::V;
	case GLFW_KEY_W: return Twisted::Key::W;
	case GLFW_KEY_X: return Twisted::Key::X;
	case GLFW_KEY_Y: return Twisted::Key::Y;
	case GLFW_KEY_Z: return Twisted::Key::Z;
	case GLFW_KEY_LEFT_BRACKET: return Twisted::Key::LeftBracket;
	case GLFW_KEY_BACKSLASH: return Twisted::Key::Backslash;
	case GLFW_KEY_RIGHT_BRACKET: return Twisted::Key::RightBracket;
	case GLFW_KEY_GRAVE_ACCENT: return Twisted::Key::GraveAccent;
	case GLFW_KEY_WORLD_1: return Twisted::Key::World1;
	case GLFW_KEY_WORLD_2: return Twisted::Key::World2;
	case GLFW_KEY_ESCAPE: return Twisted::Key::Escape;
	case GLFW_KEY_ENTER: return Twisted::Key::Enter;
	case GLFW_KEY_TAB: return Twisted::Key::Tab;
	case GLFW_KEY_BACKSPACE: return Twisted::Key::Backspace;
	case GLFW_KEY_INSERT: return Twisted::Key::Insert;
	case GLFW_KEY_DELETE: return Twisted::Key::Delete;
	case GLFW_KEY_RIGHT: return Twisted::Key::Right;
	case GLFW_KEY_LEFT: return Twisted::Key::Left;
	case GLFW_KEY_DOWN: return Twisted::Key::Down;
	case GLFW_KEY_UP: return Twisted::Key::Up;
	case GLFW_KEY_PAGE_UP: return Twisted::Key::PageUp;
	case GLFW_KEY_PAGE_DOWN: return Twisted::Key::PageDown;
	case GLFW_KEY_HOME: return Twisted::Key::Home;
	case GLFW_KEY_END: return Twisted::Key::End;
	case GLFW_KEY_CAPS_LOCK: return Twisted::Key::CapsLock;
	case GLFW_KEY_SCROLL_LOCK: return Twisted::Key::ScrollLock;
	case GLFW_KEY_NUM_LOCK: return Twisted::Key::NumLock;
	case GLFW_KEY_PRINT_SCREEN: return Twisted::Key::PrintScreen;
	case GLFW_KEY_PAUSE: return Twisted::Key::Pause;
	case GLFW_KEY_F1: return Twisted::Key::F1;
	case GLFW_KEY_F2: return Twisted::Key::F2;
	case GLFW_KEY_F3: return Twisted::Key::F3;
	case GLFW_KEY_F4: return Twisted::Key::F4;
	case GLFW_KEY_F5: return Twisted::Key::F5;
	case GLFW_KEY_F6: return Twisted::Key::F6;
	case GLFW_KEY_F7: return Twisted::Key::F7;
	case GLFW_KEY_F8: return Twisted::Key::F8;
	case GLFW_KEY_F9: return Twisted::Key::F9;
	case GLFW_KEY_F10: return Twisted::Key::F10;
	case GLFW_KEY_F11: return Twisted::Key::F11;
	case GLFW_KEY_F12: return Twisted::Key::F12;
	case GLFW_KEY_F13: return Twisted::Key::F13;
	case GLFW_KEY_F14: return Twisted::Key::F14;
	case GLFW_KEY_F15: return Twisted::Key::F15;
	case GLFW_KEY_F16: return Twisted::Key::F16;
	case GLFW_KEY_F17: return Twisted::Key::F17;
	case GLFW_KEY_F18: return Twisted::Key::F18;
	case GLFW_KEY_F19: return Twisted::Key::F19;
	case GLFW_KEY_F20: return Twisted::Key::F20;
	case GLFW_KEY_F21: return Twisted::Key::F21;
	case GLFW_KEY_F22: return Twisted::Key::F22;
	case GLFW_KEY_F23: return Twisted::Key::F23;
	case GLFW_KEY_F24: return Twisted::Key::F24;
	case GLFW_KEY_F25: return Twisted::Key::F25;
	case GLFW_KEY_KP_0: return Twisted::Key::KP_0;
	case GLFW_KEY_KP_1: return Twisted::Key::KP_1;
	case GLFW_KEY_KP_2: return Twisted::Key::KP_2;
	case GLFW_KEY_KP_3: return Twisted::Key::KP_3;
	case GLFW_KEY_KP_4: return Twisted::Key::KP_4;
	case GLFW_KEY_KP_5: return Twisted::Key::KP_5;
	case GLFW_KEY_KP_6: return Twisted::Key::KP_6;
	case GLFW_KEY_KP_7: return Twisted::Key::KP_7;
	case GLFW_KEY_KP_8: return Twisted::Key::KP_8;
	case GLFW_KEY_KP_9: return Twisted::Key::KP_9;
	case GLFW_KEY_KP_DECIMAL: return Twisted::Key::KP_Decimal;
	case GLFW_KEY_KP_DIVIDE: return Twisted::Key::KP_Divide;
	case GLFW_KEY_KP_MULTIPLY: return Twisted::Key::KP_Multiply;
	case GLFW_KEY_KP_SUBTRACT: return Twisted::Key::KP_Subtract;
	case GLFW_KEY_KP_ADD: return Twisted::Key::KP_Add;
	case GLFW_KEY_KP_ENTER: return Twisted::Key::KP_Enter;
	case GLFW_KEY_KP_EQUAL: return Twisted::Key::KP_Equal;
	case GLFW_KEY_LEFT_SHIFT: return Twisted::Key::LeftShift;
	case GLFW_KEY_LEFT_CONTROL: return Twisted::Key::LeftControl;
	case GLFW_KEY_LEFT_ALT: return Twisted::Key::LeftAlt;
	case GLFW_KEY_LEFT_SUPER: return Twisted::Key::LeftSuper;
	case GLFW_KEY_RIGHT_SHIFT: return Twisted::Key::RightShift;
	case GLFW_KEY_RIGHT_CONTROL: return Twisted::Key::RightControl;
	case GLFW_KEY_RIGHT_ALT: return Twisted::Key::RightAlt;
	case GLFW_KEY_RIGHT_SUPER: return Twisted::Key::RightSuper;
	case GLFW_KEY_MENU: return Twisted::Key::Menu;
	default:
	{
		TWISTED_WARN("Unsupported GLFW Key");
		return Twisted::Key::Invalid;
	}
	}
}

[[nodiscard]] inline int twistedToGlfwKey(Twisted::Key key) noexcept
{
	switch (key)
	{
	case Twisted::Key::Space: return GLFW_KEY_SPACE;
	case Twisted::Key::Apostrophe: return GLFW_KEY_APOSTROPHE;
	case Twisted::Key::Comma: return GLFW_KEY_COMMA;
	case Twisted::Key::Minus: return GLFW_KEY_MINUS;
	case Twisted::Key::Period: return GLFW_KEY_PERIOD;
	case Twisted::Key::Slash: return GLFW_KEY_SLASH;
	case Twisted::Key::D0: return GLFW_KEY_0;
	case Twisted::Key::D1: return GLFW_KEY_1;
	case Twisted::Key::D2: return GLFW_KEY_2;
	case Twisted::Key::D3: return GLFW_KEY_3;
	case Twisted::Key::D4: return GLFW_KEY_4;
	case Twisted::Key::D5: return GLFW_KEY_5;
	case Twisted::Key::D6: return GLFW_KEY_6;
	case Twisted::Key::D7: return GLFW_KEY_7;
	case Twisted::Key::D8: return GLFW_KEY_8;
	case Twisted::Key::D9: return GLFW_KEY_9;
	case Twisted::Key::Semicolon: return GLFW_KEY_SEMICOLON;
	case Twisted::Key::Equal: return GLFW_KEY_EQUAL;
	case Twisted::Key::A: return GLFW_KEY_A;
	case Twisted::Key::B: return GLFW_KEY_B;
	case Twisted::Key::C: return GLFW_KEY_C;
	case Twisted::Key::D: return GLFW_KEY_D;
	case Twisted::Key::E: return GLFW_KEY_E;
	case Twisted::Key::F: return GLFW_KEY_F;
	case Twisted::Key::G: return GLFW_KEY_G;
	case Twisted::Key::H: return GLFW_KEY_H;
	case Twisted::Key::I: return GLFW_KEY_I;
	case Twisted::Key::J: return GLFW_KEY_J;
	case Twisted::Key::K: return GLFW_KEY_K;
	case Twisted::Key::L: return GLFW_KEY_L;
	case Twisted::Key::M: return GLFW_KEY_M;
	case Twisted::Key::N: return GLFW_KEY_N;
	case Twisted::Key::O: return GLFW_KEY_O;
	case Twisted::Key::P: return GLFW_KEY_P;
	case Twisted::Key::Q: return GLFW_KEY_Q;
	case Twisted::Key::R: return GLFW_KEY_R;
	case Twisted::Key::S: return GLFW_KEY_S;
	case Twisted::Key::T: return GLFW_KEY_T;
	case Twisted::Key::U: return GLFW_KEY_U;
	case Twisted::Key::V: return GLFW_KEY_V;
	case Twisted::Key::W: return GLFW_KEY_W;
	case Twisted::Key::X: return GLFW_KEY_X;
	case Twisted::Key::Y: return GLFW_KEY_Y;
	case Twisted::Key::Z: return GLFW_KEY_Z;
	case Twisted::Key::LeftBracket: return GLFW_KEY_LEFT_BRACKET;
	case Twisted::Key::Backslash: return GLFW_KEY_BACKSLASH;
	case Twisted::Key::RightBracket: return GLFW_KEY_RIGHT_BRACKET;
	case Twisted::Key::GraveAccent: return GLFW_KEY_GRAVE_ACCENT;
	case Twisted::Key::World1: return GLFW_KEY_WORLD_1;
	case Twisted::Key::World2: return GLFW_KEY_WORLD_2;
	case Twisted::Key::Escape: return GLFW_KEY_ESCAPE;
	case Twisted::Key::Enter: return GLFW_KEY_ENTER;
	case Twisted::Key::Tab: return GLFW_KEY_TAB;
	case Twisted::Key::Backspace: return GLFW_KEY_BACKSPACE;
	case Twisted::Key::Insert: return GLFW_KEY_INSERT;
	case Twisted::Key::Delete: return GLFW_KEY_DELETE;
	case Twisted::Key::Right: return GLFW_KEY_RIGHT;
	case Twisted::Key::Left: return GLFW_KEY_LEFT;
	case Twisted::Key::Down: return GLFW_KEY_DOWN;
	case Twisted::Key::Up: return GLFW_KEY_UP;
	case Twisted::Key::PageUp: return GLFW_KEY_PAGE_UP;
	case Twisted::Key::PageDown: return GLFW_KEY_PAGE_DOWN;
	case Twisted::Key::Home: return GLFW_KEY_HOME;
	case Twisted::Key::End: return GLFW_KEY_END;
	case Twisted::Key::CapsLock: return GLFW_KEY_CAPS_LOCK;
	case Twisted::Key::ScrollLock: return GLFW_KEY_SCROLL_LOCK;
	case Twisted::Key::NumLock: return GLFW_KEY_NUM_LOCK;
	case Twisted::Key::PrintScreen: return GLFW_KEY_PRINT_SCREEN;
	case Twisted::Key::Pause: return GLFW_KEY_PAUSE;
	case Twisted::Key::F1: return GLFW_KEY_F1;
	case Twisted::Key::F2: return GLFW_KEY_F2;
	case Twisted::Key::F3: return GLFW_KEY_F3;
	case Twisted::Key::F4: return GLFW_KEY_F4;
	case Twisted::Key::F5: return GLFW_KEY_F5;
	case Twisted::Key::F6: return GLFW_KEY_F6;
	case Twisted::Key::F7: return GLFW_KEY_F7;
	case Twisted::Key::F8: return GLFW_KEY_F8;
	case Twisted::Key::F9: return GLFW_KEY_F9;
	case Twisted::Key::F10: return GLFW_KEY_F10;
	case Twisted::Key::F11: return GLFW_KEY_F11;
	case Twisted::Key::F12: return GLFW_KEY_F12;
	case Twisted::Key::F13: return GLFW_KEY_F13;
	case Twisted::Key::F14: return GLFW_KEY_F14;
	case Twisted::Key::F15: return GLFW_KEY_F15;
	case Twisted::Key::F16: return GLFW_KEY_F16;
	case Twisted::Key::F17: return GLFW_KEY_F17;
	case Twisted::Key::F18: return GLFW_KEY_F18;
	case Twisted::Key::F19: return GLFW_KEY_F19;
	case Twisted::Key::F20: return GLFW_KEY_F20;
	case Twisted::Key::F21: return GLFW_KEY_F21;
	case Twisted::Key::F22: return GLFW_KEY_F22;
	case Twisted::Key::F23: return GLFW_KEY_F23;
	case Twisted::Key::F24: return GLFW_KEY_F24;
	case Twisted::Key::F25: return GLFW_KEY_F25;
	case Twisted::Key::KP_0: return GLFW_KEY_KP_0;
	case Twisted::Key::KP_1: return GLFW_KEY_KP_1;
	case Twisted::Key::KP_2: return GLFW_KEY_KP_2;
	case Twisted::Key::KP_3: return GLFW_KEY_KP_3;
	case Twisted::Key::KP_4: return GLFW_KEY_KP_4;
	case Twisted::Key::KP_5: return GLFW_KEY_KP_5;
	case Twisted::Key::KP_6: return GLFW_KEY_KP_6;
	case Twisted::Key::KP_7: return GLFW_KEY_KP_7;
	case Twisted::Key::KP_8: return GLFW_KEY_KP_8;
	case Twisted::Key::KP_9: return GLFW_KEY_KP_9;
	case Twisted::Key::KP_Decimal: return GLFW_KEY_KP_DECIMAL;
	case Twisted::Key::KP_Divide: return GLFW_KEY_KP_DIVIDE;
	case Twisted::Key::KP_Multiply: return GLFW_KEY_KP_MULTIPLY;
	case Twisted::Key::KP_Subtract: return GLFW_KEY_KP_SUBTRACT;
	case Twisted::Key::KP_Add: return GLFW_KEY_KP_ADD;
	case Twisted::Key::KP_Enter: return GLFW_KEY_KP_ENTER;
	case Twisted::Key::KP_Equal: return GLFW_KEY_KP_EQUAL;
	case Twisted::Key::LeftShift: return GLFW_KEY_LEFT_SHIFT;
	case Twisted::Key::LeftControl: return GLFW_KEY_LEFT_CONTROL;
	case Twisted::Key::LeftAlt: return GLFW_KEY_LEFT_ALT;
	case Twisted::Key::LeftSuper: return GLFW_KEY_LEFT_SUPER;
	case Twisted::Key::RightShift: return GLFW_KEY_RIGHT_SHIFT;
	case Twisted::Key::RightControl: return GLFW_KEY_RIGHT_CONTROL;
	case Twisted::Key::RightAlt: return GLFW_KEY_RIGHT_ALT;
	case Twisted::Key::RightSuper: return GLFW_KEY_RIGHT_SUPER;
	case Twisted::Key::Menu: return GLFW_KEY_MENU;
	default:
	{
		TWISTED_WARN("Unsupported Twisted Key");
		return -1;
	}
	}
}


namespace Twisted
{

	Window::Window(const std::string& title, Vec2i size, Vec2i position)
	{
	}

	Window::~Window()
	{
		glfwDestroyWindow(static_cast<GLFWwindow*>(m_pointer));
	}

	Vec2i Window::GetSize()const
	{
		Vec2i size;
		glfwGetWindowSize(static_cast<GLFWwindow*>(m_pointer), &size.x, &size.y);
		return size;
	}
	Vec2i Window::GetPosition()const
	{
		Vec2i pos;
		glfwGetWindowPos(static_cast<GLFWwindow*>(m_pointer), &pos.x, &pos.y);
		return pos;
	}
	std::string Window::GetTitle()const
	{
		const char* title = glfwGetWindowTitle(static_cast<GLFWwindow*>(m_pointer));
		return std::string(title);
	}

	void Window::SetTitle(const std::string& newTitle)
	{
		glfwSetWindowTitle(static_cast<GLFWwindow*>(m_pointer), newTitle.c_str());
	}
	void Window::SetSize(Vec2i newSize)
	{
		glfwSetWindowSize(static_cast<GLFWwindow*>(m_pointer), newSize.x, newSize.y);
	}
	void Window::SetPosition(Vec2i newPosition)
	{
		glfwSetWindowPos(static_cast<GLFWwindow*>(m_pointer), newPosition.x, newPosition.y);
	}

	void Window::PollEvents()
	{
		Input::GetInstance().ResetInput();
		glfwPollEvents();
	}


	void Window::SetFullscreen()
	{
		//MonitorInfo monitor = MonitorInfo::GetPrimaryMonitor();

		//glfwSetWindowMonitor(m_impl->Pointer, glfwGetPrimaryMonitor(), 100, 100, 300, 300, monitor.GetRefreshRate());
	}

	void Window::SetWindowed(Vec2i size, Vec2i pos)
	{
		glfwSetWindowMonitor(static_cast<GLFWwindow*>(m_pointer), nullptr, pos.x, pos.y, size.x, size.y, GLFW_DONT_CARE);
	}

	bool Window::CloseWindow()
	{
		glfwSetWindowShouldClose(static_cast<GLFWwindow*>(m_pointer), GLFW_TRUE);

		TWISTED_INFO("Window closed");
		return true;
	}

	void* Window::GetNativeHandle()
	{
		return glfwGetWin32Window(static_cast<GLFWwindow*>(m_pointer));
	}

	void* Window::GetContextAdress()
	{
		return glfwGetProcAddress;
	}
	void* Window::GetRawPointer()
	{
		return static_cast<GLFWwindow*>(m_pointer);
	}

	URef<Window> Window::CreateNewWindow(const std::string& title, Vec2i size, Vec2i position)
	{
		GLFWwindow* windowPointer = nullptr;
		windowPointer = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, NULL);
		if (!windowPointer)
		{
			TWISTED_ERROR("CreateNewWindow() failure! GLFW window pointer creation failure");
			return nullptr;
		}

		URef<Window> window = std::make_unique<Window>(title, size, position);
		window->m_pointer = windowPointer;
		window->m_context = std::make_unique<GraphicsContext>(windowPointer);
		glfwMakeContextCurrent(windowPointer);
		glfwSetWindowUserPointer(windowPointer, window.get());
		glfwSetWindowPos(windowPointer, position.x, position.y);

		glfwSetWindowCloseCallback(windowPointer,
			[](GLFWwindow* pointer)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				window->CloseWindowEvent.Invoke();
			});
		glfwSetWindowSizeCallback(windowPointer,
			[](GLFWwindow* pointer, int newWidth, int newHeight)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));
				//window->m_frameBuffer->Resize(newWidth, newHeight);
				window->WindowResizeEvent.Invoke();
			});
		glfwSetKeyCallback(windowPointer,
			[](GLFWwindow* pointer, int glfwKey, int glfwScancode, int glfwAction, int glfwMods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));

				Twisted::Key key = glfwToTwistedKey(glfwKey);

				bool isDown;
				if (glfwAction == GLFW_PRESS)
					isDown = true;
				else if (glfwAction == GLFW_RELEASE)
					isDown = false;
				else
					return;

				Input::GetInstance().UpdateKey(key, isDown);
			});
		glfwSetMouseButtonCallback(windowPointer,
			[](GLFWwindow* pointer, int glfwButton, int glfwAction, int glfwMods)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pointer));

				Twisted::MouseButton button = glfwToTwistedButton(glfwButton);
				bool isDown;
				if (glfwAction == GLFW_PRESS)
					isDown = true;
				else if (glfwAction == GLFW_RELEASE)
					isDown = false;
				else
					return;
				Input::GetInstance().UpdateMouseButton(button, isDown);
			});
		glfwSetCursorPosCallback(windowPointer,
			[](GLFWwindow* pointer, double xPos, double yPos)
			{
				Input& input = Input::GetInstance();
				input.m_mousePosition[1] = input.m_mousePosition[0];
				input.m_mousePosition[0] = { static_cast<float>(xPos), static_cast<float>(yPos) };
			});

		TWISTED_INFO("Window Created");
		return window;
	}
}

#endif

#pragma once

#include "KeyCodes.h"
#include "ButtonCodes.h"
#include "Utils/GlmUtils.h"
#include <variant>

namespace Twisted
{
	struct KeyEvent { Key key; KeyState state; };
	struct MouseButtonEvent { MouseButton button; KeyState state; };
	struct MouseMoveEvent { Vec2f position; };
	struct MouseWheelEvent { float delta; };
	struct WindowCloseEvent {};
	struct WindowResizeEvent { Vec2i size; };
	struct WindowFocusEvent { bool gained; }; // covers killfocus too

	using WindowEvent = std::variant<
		KeyEvent,
		MouseButtonEvent,
		MouseMoveEvent,
		MouseWheelEvent,
		WindowCloseEvent,
		WindowResizeEvent,
		WindowFocusEvent
	>;
}
#pragma once

#include "Utils/Event.h"
#include "ButtonCodes.h"
#include "KeyCodes.h"

namespace Twisted
{
	struct WindowEventRegistry
	{
		Event<MouseButton, KeyState> MouseButtonEvent;
		Event<Key, KeyState> KeyEvent;
		Event<float> MouseWheelEvent;
		Event<float> MousePos;
	};
}
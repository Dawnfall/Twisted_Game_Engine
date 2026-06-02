#pragma once
#include "AppCore.h"
#include <cstdint>

namespace Twisted
{
	enum class MouseButton :uint8_t
	{
		Left = 0,
		Right,
		Middle,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
		Invalid,
		Count=Invalid
	};
}


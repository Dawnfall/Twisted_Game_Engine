#include "Twisted/Windowing/Input.h"

#include "Debug/Logger.h"

namespace Twisted
{
	void Input::UpdateKey(Twisted::Key key, Twisted::KeyState state)
	{
		if (key == Key::Invalid)
			return;
		if (state == KeyState::INVALID)
			return;

		size_t idx = static_cast<size_t>(key);
		if (state == KeyState::PRESSED)
		{
			if (!m_pressedKeys.test(idx))
				m_justPressedKeys.set(idx);
			m_pressedKeys.set(idx);
		}
		else
		{
			if (m_pressedKeys.test(idx))
				m_justReleasedKeys.set(idx);
			m_pressedKeys.reset(idx);
		}
	}
	void Input::UpdateMouseButton(Twisted::MouseButton button, Twisted::KeyState state)
	{
		if (button == MouseButton::Invalid)
			return;

		const uint16_t mask = 1 << static_cast<uint8_t>(button);
		if (state == KeyState::PRESSED)
		{
			if ((m_pressedButtons & mask) == 0) // first frame pressed
				m_justPressedButtons |= mask;
			m_pressedButtons |= mask;
		}
		else
		{
			if (m_pressedButtons & mask) // was pressed, now released
				m_justReleasedButtons |= mask;
			m_pressedButtons &= ~mask;
		}
	}
	void Input::UpdateMousePosition(float x, float y)
	{
		m_mouseDelta.x = x - m_mousePos.x;
		m_mouseDelta.y = y - m_mousePos.y;

		m_mousePos.x = x;
		m_mousePos.y = y;
	}

	void Input::UpdateMouseWheel(float delta)
	{
		m_wheelDelta = delta;
	}


}


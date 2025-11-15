#include "Input.h"

#include "Debug/Logger.h"

namespace Twisted
{
	void Input::UpdateKey(Twisted::Key key, bool isDown)
	{
		if (key == Key::Invalid)
			return;

		size_t idx = static_cast<size_t>(key);
		if (isDown)
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
	void Input::UpdateMouseButton(Twisted::MouseButton button, bool isDown)
	{
		if (button == MouseButton::Invalid)
			return;

		const uint16_t mask = 1 << static_cast<uint8_t>(button);
		if (isDown)
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
		m_mousePosition[1] = m_mousePosition[0];
		m_mousePosition[0].x = x;
		m_mousePosition[0].y = y;
	}

}


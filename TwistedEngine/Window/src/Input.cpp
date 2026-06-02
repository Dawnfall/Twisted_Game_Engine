#include "Input.h"

#include "Debug/Logger.h"

namespace Twisted
{
	Input& Input::GetInstance()
	{
		static Input instance;
		return instance;
	}

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

		const uint16_t mask = static_cast<uint16_t>(1u << static_cast<uint8_t>(button));
		const size_t idx = static_cast<size_t>(button);

		if (state == KeyState::PRESSED)
		{
			if ((m_pressedButtons & mask) == 0)
			{
				m_justPressedButtons |= mask;
				m_buttonPressPos[idx] = m_mousePos;
				m_dragging[idx] = false;
			}
			m_pressedButtons |= mask;
		}
		else
		{
			if (m_pressedButtons & mask)
			{
				m_justReleasedButtons |= mask;
				if (m_dragging[idx])
				{
					m_dragEnd[idx] = true;
					m_dragging[idx] = false;
				}
				else
				{
					m_clicked[idx] = true;
				}
			}
			m_pressedButtons &= ~mask;
		}
	}
	void Input::UpdateMousePosition(float x, float y)
	{
		m_mouseDelta.x = x - m_mousePos.x;
		m_mouseDelta.y = y - m_mousePos.y;
		m_mousePos.x = x;
		m_mousePos.y = y;

		for (size_t i = 0; i < ButtonCount; ++i)
		{
			const uint16_t mask = static_cast<uint16_t>(1u << static_cast<uint8_t>(i));
			if ((m_pressedButtons & mask) && !m_dragging[i])
			{
				float dx = x - m_buttonPressPos[i].x;
				float dy = y - m_buttonPressPos[i].y;
				if (dx * dx + dy * dy >= DragThreshold * DragThreshold)
				{
					m_dragStart[i] = true;
					m_dragging[i] = true;
				}
			}
		}
	}

	void Input::UpdateMouseWheel(float delta)
	{
		m_wheelDelta = delta;
	}


}


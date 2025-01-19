#include "renderingpch.h"

#include "InputManager.h"

namespace Twisted
{
	void InputManager::Clear(GLFWwindow* window)
	{
		m_justPressedKeys.clear();
		m_justReleasedKeys.clear();
		m_mousePosition[1] = m_mousePosition[0];

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		m_mousePosition[0] = Vec2f(static_cast<float>(x), static_cast<float>(y));
	}

	void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_UNKNOWN)
			return;

		switch (action)
		{
		case GLFW_PRESS:
			if (!m_pressedKeys.contains(key))
			{
				m_pressedKeys.emplace(key);
				m_justPressedKeys.emplace(key);
			}
			break;
		case GLFW_RELEASE:
			if (m_pressedKeys.contains(key))
			{
				m_pressedKeys.erase(key);
				m_justReleasedKeys.emplace(key);
			}
			break;
		}
	}
	void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_KEY_UNKNOWN)
			return;

		switch (action)
		{
		case GLFW_PRESS:
			if (!m_pressedMouse.contains(button))
			{
				m_pressedMouse.emplace(button);
				m_justPressedMouse.emplace(button);
			}
			break;
		case GLFW_RELEASE:
			if (m_pressedMouse.contains(button))
			{
				m_pressedMouse.erase(button);
				m_justReleasedMouse.emplace(button);
			}
			break;
		}
	}
}
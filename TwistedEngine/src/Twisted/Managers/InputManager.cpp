#include "pch.h"
#include "InputManager.h"

namespace Twisted
{
	void InputManager::Clear()
	{
		m_justPressedKeys.clear();
		m_justReleasedKeys.clear();

		//TODO:... glfwGetCursorPos()
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
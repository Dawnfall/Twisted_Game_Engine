#pragma once
#include "Collections/Vector.h"

#include <GLFW/glfw3.h>


namespace Twisted
{
	class InputManager
	{
	public:
		void Clear();

		bool GetKeyDown(int key) { return m_justPressedKeys.contains(key); }
		bool GeKeyUp(int key) { return m_justReleasedKeys.contains(key); }
		bool GetKey(int key) { return m_pressedKeys.contains(key); }

		bool GetMouseButtonDown(int key) { return m_justPressedMouse.contains(key); }
		bool GetMouseButtonUp(int key) { return m_justReleasedMouse.contains(key); }
		bool GetMouseButton(int key) { return m_pressedMouse.contains(key); }

		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		
		const Vec2d& GetMousePosition()const { return m_mousePosition; }

	private:
		std::unordered_set<int> m_pressedKeys;
		std::unordered_set<int> m_justPressedKeys;
		std::unordered_set<int> m_justReleasedKeys;

		std::unordered_set<int> m_pressedMouse;
		std::unordered_set<int> m_justPressedMouse;
		std::unordered_set<int> m_justReleasedMouse;
		
		Vec2d m_mousePosition;
	};


}
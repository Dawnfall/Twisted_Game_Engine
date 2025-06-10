#pragma once
#include "Utils/GlmUtils.h"
#include "AppCore.h"
#include "Data/Event.h"
#include <unordered_set>

class GLFWwindow;

namespace Twisted
{
	class TWISTED_API InputManager
	{
	public:
		Event<int> KeyCallbackEvent;
		Event<int> ButtonCallbackEvent;
	public:
		void Clear(GLFWwindow* window);

		bool GetKeyDown(int key) { return m_justPressedKeys.contains(key); }
		bool GeKeyUp(int key) { return m_justReleasedKeys.contains(key); }
		bool GetKey(int key) { return m_pressedKeys.contains(key); }

		bool GetMouseButtonDown(int key) { return m_justPressedMouse.contains(key); }
		bool GetMouseButtonUp(int key) { return m_justReleasedMouse.contains(key); }
		bool GetMouseButton(int key) { return m_pressedMouse.contains(key); }

		const Vec2f& GetMousePosition()const { return m_mousePosition[0]; }
		const Vec2f GetMouseDelta()const { return m_mousePosition[0] - m_mousePosition[1]; }

		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	private:

		std::unordered_set<int> m_pressedKeys;
		std::unordered_set<int> m_justPressedKeys;
		std::unordered_set<int> m_justReleasedKeys;

		std::unordered_set<int> m_pressedMouse;
		std::unordered_set<int> m_justPressedMouse;
		std::unordered_set<int> m_justReleasedMouse;

		Vec2f m_mousePosition[2]{ {0,0},{0,0} };
	};


}
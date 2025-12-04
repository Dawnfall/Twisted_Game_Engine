#pragma once
#include "AppCore.h"

#include "ButtonCodes.h"
#include "KeyCodes.h"
#include "Utils/GlmUtils.h"

#include <bitset>

namespace Twisted
{
	class TWISTED_API Input
	{
	public:
		inline static Input& GetInstance()
		{
			static Input instance;
			return instance;
		}

		bool GetMouseButtonDown(MouseButton button)const { return (m_justPressedButtons & (1 << static_cast<uint8_t>(button))) != 0; } //true on pressed frame
		bool GetMouseButtonUp(MouseButton button)const { return (m_justReleasedButtons & (1 << static_cast<uint8_t>(button))) != 0; } //true on release frame
		bool GetMouseButton(MouseButton button)const { return (m_pressedButtons & (1 << static_cast<uint8_t>(button))) != 0; } //is still pressed

		bool GetKeyDown(Key key) const
		{
			if (key == Key::Invalid)
				return false;
			return m_justPressedKeys.test(static_cast<size_t>(key));
		}
		bool GetKeyUp(Key key) const
		{
			if (key == Key::Invalid)
				return false;
			return m_justReleasedKeys.test(static_cast<size_t>(key));
		}
		bool GetKey(Key key)const
		{
			if (key == Key::Invalid)
				return false;
			return m_pressedKeys.test(static_cast<size_t>(key));
		}

		const Vec2f& GetMousePosition()const { return m_mousePos; }
		const Vec2f& GetMouseDelta()const { return m_mouseDelta; }


		void UpdateKey(Twisted::Key key, bool isDown);
		void UpdateMouseButton(Twisted::MouseButton button, bool isDown);
		void UpdateMousePosition(float x, float y);
	private:

		void ResetInput()
		{
			m_justPressedButtons = 0;
			m_justReleasedButtons = 0;

			m_justPressedKeys.reset();
			m_justReleasedKeys.reset();
		}



	private:
		uint16_t m_pressedButtons = 0;
		uint16_t m_justPressedButtons = 0;
		uint16_t m_justReleasedButtons = 0;

		std::bitset<static_cast<size_t>(Key::Count)> m_pressedKeys{};
		std::bitset<static_cast<size_t>(Key::Count)> m_justPressedKeys{};
		std::bitset<static_cast<size_t>(Key::Count)> m_justReleasedKeys{};

		Vec2f m_mousePos;
		Vec2f m_mouseDelta;

	private:
		Input() = default;

	public:
		Input(const Input& other) = delete;
		Input(Input&& other) = delete;
		Input& operator=(const Input& other) = delete;
		Input& operator=(Input&& other) = delete;
	};
}


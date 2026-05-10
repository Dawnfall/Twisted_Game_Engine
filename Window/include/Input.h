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
		static constexpr float DragThreshold = 5.0f;

		inline static Input& GetInstance()
		{
			static Input instance;
			return instance;
		}

		// Raw state
		bool GetMouseButtonDown(MouseButton button)const { return (m_justPressedButtons & (1 << static_cast<uint8_t>(button))) != 0; }
		bool GetMouseButtonUp(MouseButton button)const { return (m_justReleasedButtons & (1 << static_cast<uint8_t>(button))) != 0; }
		bool GetMouseButton(MouseButton button)const { return (m_pressedButtons & (1 << static_cast<uint8_t>(button))) != 0; }

		bool GetKeyDown(Key key) const
		{
			if (key == Key::Invalid) return false;
			return m_justPressedKeys.test(static_cast<size_t>(key));
		}
		bool GetKeyUp(Key key) const
		{
			if (key == Key::Invalid) return false;
			return m_justReleasedKeys.test(static_cast<size_t>(key));
		}
		bool GetKey(Key key) const
		{
			if (key == Key::Invalid) return false;
			return m_pressedKeys.test(static_cast<size_t>(key));
		}

		const Vec2f& GetMousePosition() const { return m_mousePos; }
		const Vec2f& GetMouseDelta() const { return m_mouseDelta; }
		float GetMouseWheelDelta() const { return m_wheelDelta; }

		// Gesture state (click = press+release without drag, drag threshold = DragThreshold px)
		bool IsClicked(MouseButton button = MouseButton::Left) const
		{
			if (button == MouseButton::Invalid) return false;
			return m_clicked[static_cast<size_t>(button)];
		}
		bool IsDragStart(MouseButton button = MouseButton::Left) const
		{
			if (button == MouseButton::Invalid) return false;
			return m_dragStart[static_cast<size_t>(button)];
		}
		bool IsDragging(MouseButton button = MouseButton::Left) const
		{
			if (button == MouseButton::Invalid) return false;
			return m_dragging[static_cast<size_t>(button)];
		}
		bool IsDragEnd(MouseButton button = MouseButton::Left) const
		{
			if (button == MouseButton::Invalid) return false;
			return m_dragEnd[static_cast<size_t>(button)];
		}

		void UpdateKey(Key key, KeyState state);
		void UpdateMouseButton(MouseButton button, KeyState state);
		void UpdateMousePosition(float x, float y);
		void UpdateMouseWheel(float delta);

		void BeginFrame()
		{
			m_justPressedButtons = 0;
			m_justReleasedButtons = 0;
			m_wheelDelta = 0;
			m_mouseDelta = Constants::Vec2Zero;
			m_justPressedKeys.reset();
			m_justReleasedKeys.reset();

			for (size_t i = 0; i < ButtonCount; ++i)
			{
				m_clicked[i] = false;
				m_dragStart[i] = false;
				m_dragEnd[i] = false;
			}
		}

		void ClearAllInput()
		{
			m_pressedButtons = 0;
			m_justPressedButtons = 0;
			m_justReleasedButtons = 0;
			m_wheelDelta = 0;
			m_mouseDelta = Constants::Vec2Zero;
			m_pressedKeys.reset();
			m_justPressedKeys.reset();
			m_justReleasedKeys.reset();

			for (size_t i = 0; i < ButtonCount; ++i)
			{
				m_clicked[i] = false;
				m_dragStart[i] = false;
				m_dragging[i] = false;
				m_dragEnd[i] = false;
			}
		}

	private:
		static constexpr size_t ButtonCount = static_cast<size_t>(MouseButton::Count);

		uint16_t m_pressedButtons = 0;
		uint16_t m_justPressedButtons = 0;
		uint16_t m_justReleasedButtons = 0;

		std::bitset<static_cast<size_t>(Key::Count)> m_pressedKeys{};
		std::bitset<static_cast<size_t>(Key::Count)> m_justPressedKeys{};
		std::bitset<static_cast<size_t>(Key::Count)> m_justReleasedKeys{};

		Vec2f m_mousePos = Constants::Vec2Zero;
		Vec2f m_mouseDelta = Constants::Vec2Zero;
		float m_wheelDelta = 0;

		Vec2f m_buttonPressPos[ButtonCount] = {};
		bool m_dragging[ButtonCount] = {};
		bool m_clicked[ButtonCount] = {};
		bool m_dragStart[ButtonCount] = {};
		bool m_dragEnd[ButtonCount] = {};

	private:
		Input() :m_mousePos(Constants::Vec2Zero), m_mouseDelta(Constants::Vec2Zero) {}

	public:
		Input(const Input& other) = delete;
		Input(Input&& other) = delete;
		Input& operator=(const Input& other) = delete;
		Input& operator=(Input&& other) = delete;
	};
}


#pragma once
#include "AppCore.h"
#include "Utils/Event.h"
#include "Windows/Monitor.h"
#include "Windows/InputManager.h"
#include "Utils/Color.h"

namespace Twisted
{
	class Window
	{
	public:
		Window(GLFWwindow* window, const std::string& title, Vec2i size, Vec2i position);
		Window(GLFWwindow* window, const std::string& title, SRef<Monitor> monitor);

		InputManager& GetInput() { return m_input; }

		Event<> CloseWindowEvent;
		Event<> WindowResizeEvent;
		Event<int> KeyCallbackEvent;
		Event<int> ButtonCallbackEvent;

		GLFWwindow* GetPointer() { return m_pointer; }
		const std::string& GetTitle() const { return m_title; }
		Vec2i GetSize()const { return m_size; }
		Vec2i GetPosition() const { return m_pos; }

		void SetTitle(const std::string& newName);
		void SetSize(Vec2i newSize);
		void SetPosition(Vec2i newPosition);
		void SetFullscreen(SRef<Monitor> monitor);
		void SetWindowed(Vec2i size, Vec2i pos);

		void SetViewPort(float width, float height);
		void SetVsync(int deltaFrames);

		void ClearWindow(Colors::Color color)const;
		void SwapBuffers();
		void PollEvents();

		bool CloseWindow();

		static SRef<Window> CreateNewWindow(const std::string& title, SRef<Monitor> monitor, Vec2i size, Vec2i position);

	private:
		void SetCallbacks();

		InputManager m_input;
		std::string m_title;

		Vec2i m_size = Vec2i(0, 0);
		Vec2i m_pos = Vec2i(0, 0);
		GLFWwindow* m_pointer = nullptr;
	};
}

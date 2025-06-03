#pragma once
#include "AppCore.h"
#include "Utils/Event.h"
#include "Monitor.h"
#include "Utils/Color.h"
#include "Rendering/Framebuffer.h"

class GLFWwindow;

namespace Twisted
{
	bool TWISTED_API InitGLFW();
	void TWISTED_API TerminateGLFW();

	//void glfwErrorCallback(int code, const char* description);


	class TWISTED_API Window
	{
	public:
		Window(GLFWwindow* window, const std::string& title, Vec2i size, Vec2i position);
		//Window(GLFWwindow* window, const std::string& title, SRef<Monitor> monitor);

		GLFWwindow* GetPointer() { return m_pointer; }

		Vec2i GetSize()const;
		Vec2i GetPosition()const;
		std::string GetTitle()const;

		void SetTitle(const std::string& newName);
		void SetSize(Vec2i newSize);
		void SetPosition(Vec2i newPosition);
		void SetFullscreen(SRef<Monitor> monitor);
		void SetWindowed(Vec2i size, Vec2i pos);
		void SetVsync(int deltaFrames);

		void Clear(const Color& color);
		void SwapBuffers();
		void PollEvents();

		bool CloseWindow();

		Event<> CloseWindowEvent;
		Event<> WindowResizeEvent;
	
		static URef<Window> CreateNewWindow(const std::string& title, SRef<Monitor> monitor, Vec2i size, Vec2i position);
	private:
		GLFWwindow* m_pointer = nullptr;
		//URef<FrameBuffer> m_frameBuffer;
	};
}

#include "Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Dawn/Core_macros.h"
#include "Debug/Logger.h"

//TODO:... error handling

namespace Dawn
{

	Window::Window(const WindowData& windowData) :
		m_windowData(windowData),
		m_window(nullptr)
	{
		m_window = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_window); //TODO: not sure if needed

		glfwSetWindowUserPointer(m_window, this);
		glfwSetWindowCloseCallback(m_window, Window::WindowCloseCallback);

		m_renderContext = std::make_unique<RenderContext>();
		m_renderContext->Init(this);

		DAWN_INFO("Window Created");

	}

	Window::~Window()
	{
		Terminate();
	}

	void Window::OnUpdate()
	{



		glfwSwapBuffers(m_window);
		ProcessEvents();
	}

	void Window::ProcessEvents()
	{
		glfwPollEvents();
	}

	void Window::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	bool Window::Init()
	{
		if (!glfwInit())
			return false;

		glfwSetErrorCallback(Window::ErrorCallback);
		return true;
	}

	void Window::Terminate()
	{
		glfwTerminate();
	}

	Window* Window::Create(const WindowData& windowData)
	{
		return new Window(windowData);
	}

	//********************
	// glfw Callbacks

	bool Window::IsKeyPressed(GLFWwindow* window, int key)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS)
			return true;
		return false;
	}
	bool Window::IsKeyReleased(GLFWwindow* window, int key)
	{
		if (glfwGetKey(window, key) == GLFW_RELEASE)
			return true;
		return false;
	}
	void Window::ErrorCallback(int code, const char* description)
	{
		DAWN_ERROR(description);
	}
	void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//use bind to set callbacks
		//...... TODO:.....

	}
	void Window::WindowCloseCallback(GLFWwindow* window)
	{
		Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		thisWindow->closeWindowEvent.Invoke();
	}
	void Window::WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}
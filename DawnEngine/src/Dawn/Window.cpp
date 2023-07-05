#include "Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Core_macros.h"

#include "../Logger.h"

//TODO:... error handling

namespace Dawn
{

	Window::Window(const WindowData& windowData) :
		m_windowData(windowData),
		m_window(nullptr)
	{
		m_window = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_window);

		//TODO: prolly init in context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//if error
		}

		glfwSetWindowCloseCallback(m_window, Window::WindowCloseCallback);
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	}

	Window::~Window()
	{
		Terminate();
	}

	void Window::OnUpdate()
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(m_window);
		ProcessEvents();
	}

	void Window::ProcessEvents()
	{
		glfwPollEvents();
	}





	bool Window::Init()
	{
		if (!glfwInit())
			return false;

		glfwSetErrorCallback(Window::ErrorCallback);
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
	// Callbacks

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
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		//TODO:
	}
	void Window::WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}
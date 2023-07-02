#include "Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Core_macros.h"

namespace Dawn 
{

	Window::Window(const WindowData& windowData) :
		m_windowData(windowData),
		m_window(nullptr)
	{
		auto res = glfwInit();
		DAWN_DEBUG_ASSERT(res);

		m_window = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//if error
		}

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(m_window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	Window* Window::Create(const WindowData& windowData)
	{
		return new Window(windowData);
	}
}